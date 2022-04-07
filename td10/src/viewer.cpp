#include "viewer.h"
#include "SOIL2.h"
#include "camera.h"

using namespace Eigen;

Viewer::Viewer()
    : _winWidth(0)
    , _winHeight(0)
    , _wireframe(false)
{
    _IK_target.setZero();
}

Viewer::~Viewer()
{
}

////////////////////////////////////////////////////////////////////////////////
// GL stuff

// initialize OpenGL context
void Viewer::init(int w, int h)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    loadShaders();

    if (!_scene.load(DATA_DIR "/models/scene.obj"))
        exit(1);
    _scene.init();

    if (!_jointMesh.load(DATA_DIR "/models/joint.obj"))
        exit(1);
    _jointMesh.init();

    if (!_sphere.load(DATA_DIR "/models/sphere.obj"))
        exit(1);
    _sphere.init();

    if (!_segmentMesh.load(DATA_DIR "/models/segment.obj"))
        exit(1);
    _segmentMesh.init();

    if (!_cylinder.load(DATA_DIR "/models/grid.obj"))
        exit(1);
    _cylinder.init();

    _texid = SOIL_load_OGL_texture(DATA_DIR "/textures/rainbow.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    if (_texid == 0)
        printf("SOIL loading error: '%s'\n", SOIL_last_result());

    reshape(w, h);
    _cam.setPerspective(float(M_PI) / 3.f, 0.3f, 20000.0f);
    _cam.lookAt(Vector3f(0, -6, 8), Vector3f(0, 0, 0), Vector3f(0, 0, 1));
    _trackball.setCamera(&_cam);

    int nbSegments = 3;
    _lengths.resize(nbSegments);
    _lengths << 1.7f, 1.5f, 1.2f;
    _jointAngles.resize(2, nbSegments);
    _jointAngles << 45, 0, -30,
        30, 20, -20;
    _jointAngles *= float(M_PI) / 180.f;

    glEnable(GL_DEPTH_TEST);
}

void Viewer::reshape(int w, int h)
{
    _winWidth = w;
    _winHeight = h;
    _cam.setViewport(w, h);
}

void Viewer::drawScene()
{
    /**
     * COMMON
     */

    glViewport(0, 0, _winWidth, _winHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /**
     * SHADER 1
     */

    _shader.activate();

    glUniform1i(_shader.getUniformLocation("wireframe"), 0);
    glUniformMatrix4fv(_shader.getUniformLocation("view_mat"), 1, GL_FALSE, _cam.viewMatrix().data());
    glUniformMatrix4fv(_shader.getUniformLocation("proj_mat"), 1, GL_FALSE, _cam.projectionMatrix().data());

    Vector3f lightDir = Vector3f(1, 0, 1).normalized();
    lightDir = (_cam.viewMatrix().topLeftCorner(3, 3) * lightDir).normalized();
    glUniform3fv(_shader.getUniformLocation("lightDir"), 1, lightDir.data());

    Affine3f T_shader1;
    T_shader1.setIdentity();
    setObjectMatrix(T_shader1.matrix());
    glUniform3fv(_shader.getUniformLocation("color"), 1, Vector3f(0.4, 0.4, 0.8).data());
    _scene.draw(_shader);

    drawArticulatedArm();

    if (_IK_target.norm() > 0) {
        setObjectMatrix((T_shader1 * Translation3f(_IK_target) * Scaling(0.2f)).matrix());
        glUniform3fv(_shader.getUniformLocation("color"), 1, Vector3f(0.4f, 0.8f, 0.4f).data());
        _sphere.draw(_shader);
    }

    if (_wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_LINE_SMOOTH);
        glDepthFunc(GL_LEQUAL);
        glUniform1i(_shader.getUniformLocation("wireframe"), 1);

        Affine3f T_shader1_wireframe;
        T_shader1_wireframe.setIdentity();
        setObjectMatrix(T_shader1_wireframe.matrix());
        glUniform3fv(_shader.getUniformLocation("color"), 1, Vector3f(0.4f, 0.4f, 0.8f).data());
        _scene.draw(_shader);

        drawArticulatedArm();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniform1i(_shader.getUniformLocation("wireframe"), 0);
    }

    _shader.deactivate();

    /**
     * SHADER 2
     */

    _shader2.activate();

    glUniformMatrix4fv(_shader2.getUniformLocation("view_mat"), 1, GL_FALSE, _cam.viewMatrix().data());
    glUniformMatrix4fv(_shader2.getUniformLocation("proj_mat"), 1, GL_FALSE, _cam.projectionMatrix().data());

    Affine3f T_shader2 = Affine3f::Identity();

    glUniformMatrix4fv(_shader2.getUniformLocation("obj_mat"), 1, GL_FALSE, T_shader2.matrix().data());

    Matrix4f matLocal2Cam = _cam.viewMatrix() * T_shader2.matrix();
    Matrix3f matN = matLocal2Cam.topLeftCorner<3, 3>().inverse().transpose();
    glUniformMatrix3fv(_shader2.getUniformLocation("normal_mat"), 1, GL_FALSE, matN.data());

    glUniform3fv(_shader2.getUniformLocation("lightDir"), 1, lightDir.data());

    glBindTexture(GL_TEXTURE_2D, _texid);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(_shader2.getUniformLocation("colormap"), 0);

    glUniform3fv(_shader2.getUniformLocation("L"), 1, _lengths.data());

    glUniformMatrix4fv(_shader2.getUniformLocation("M"), 3, GL_FALSE, (GLfloat*)_M);

    _cylinder.draw(_shader2);

    _shader2.deactivate();
}

void Viewer::setObjectMatrix(const Matrix4f& transform_matrix) const
{
    glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, transform_matrix.data());
    Matrix4f matLocal2Cam = _cam.viewMatrix() * transform_matrix;
    Matrix3f matN = matLocal2Cam.topLeftCorner<3, 3>().inverse().transpose();
    glUniformMatrix3fv(_shader.getUniformLocation("normal_mat"), 1, GL_FALSE, matN.data());
}

void Viewer::drawArticulatedArm()
{
    const float jointScale = .2f;

    const int n = _lengths.size();

    const Vector3f phi = _jointAngles.row(0);
    const Vector3f theta = _jointAngles.row(1);

    Affine3f T { Affine3f::Identity() };
    Affine3f B { Affine3f::Identity() };

    for (int i = 0; i < n; ++i) {
        T *= AngleAxisf(phi[i], Vector3f::UnitZ());

        setObjectMatrix((T * Scaling(jointScale)).matrix());
        glUniform3fv(_shader.getUniformLocation("color"), 1, Vector3f(0.8f, 0.4f, 0.4f).data());
        _jointMesh.draw(_shader);

        T *= AngleAxisf(theta[i], Vector3f::UnitY());

        setObjectMatrix((T * Scaling(1.f, 1.f, _lengths[i])).matrix());
        glUniform3fv(_shader.getUniformLocation("color"), 1, Vector3f(0.8f, 0.8f, 0.4f).data());
        _segmentMesh.draw(_shader);

        _M[i] = T * B.inverse();

        T *= Translation3f(0, 0, _lengths[i]);
        B *= Translation3f(0, 0, _lengths[i]);
    }
}

void Viewer::updateAndDrawScene()
{
    if (_IK_target.norm() > 0) {
        /**
         * Retrieve essential data.
         */

        const int n = _lengths.size();

        const Vector3f phi = _jointAngles.row(0);
        const Vector3f theta = _jointAngles.row(1);

        /**
         * Compute positions (Eigen Library).
         */

        Vector3f P[4];
        Vector3f axes[3][2];
        Affine3f transform_matrix = Affine3f::Identity();

        for (int i = 0; i < 3; i++) {
            P[i] = transform_matrix.translation();

            transform_matrix *= AngleAxisf(phi[i], Vector3f::UnitZ());
            axes[i][0] = transform_matrix.linear().col(2);

            transform_matrix *= AngleAxisf(theta[i], Vector3f::UnitY());
            axes[i][1] = transform_matrix.linear().col(1);

            transform_matrix *= Translation3f(0, 0, _lengths[i]);
        }

        P[3] = transform_matrix.translation();

        /**
         * Compute Jacobian matrix.
         */

        Matrix<float, 3, 6> J;
        for (int i = 0; i < 3; i++) {
            J.col(2 * i) = axes[i][0].cross(P[3] - P[i]);
            J.col(2 * i + 1) = axes[i][1].cross(P[3] - P[i]);
        }

        /**
         * Update.
         */

        float delta_t = 0.01 * M_PI;
        Vector3f delta_p = _IK_target - P[3];
        VectorXf G = J.transpose() * delta_p;
        Map<VectorXf> Q = VectorXf::Map(_jointAngles.data(), 6);
        Q += delta_t * G;
    }

    drawScene();
}

void Viewer::loadShaders()
{
    // Here we can load as many shaders as we want, currently we have only one:
    _shader.loadFromFiles(DATA_DIR "/shaders/simple.vert", DATA_DIR "/shaders/simple.frag");
    _shader2.loadFromFiles(DATA_DIR "/shaders/cylinder.vert", DATA_DIR "/shaders/cylinder.frag");
    checkError();
}

bool Viewer::pickAt(const Vector2f& p, Hit& hit) const
{
    Matrix4f proj4 = _cam.projectionMatrix();
    Matrix3f proj3;
    proj3 << proj4.topLeftCorner<2, 3>(),
        proj4.bottomLeftCorner<1, 3>();
    Matrix4f C = _cam.viewMatrix().inverse();

    Vector3f q((2.0f * float(p.x() + 0.5f) / float(_winWidth) - 1.f),
        -(2.0f * float(p.y() + 0.5f) / float(_winHeight) - 1.f),
        1);

    Ray ray;
    ray.origin = C.col(3).head<3>();
    ray.direction = C.topLeftCorner<3, 3>() * (proj3.inverse() * q);
    return _scene.intersect(ray, hit);
}

////////////////////////////////////////////////////////////////////////////////
// Events

/*!
   callback to manage mouse : called when user press or release mouse button
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::mousePressed(GLFWwindow* /*window*/, int /*button*/, int action, int mods)
{
    if (((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL) && (action == GLFW_PRESS)) {
        // picking

        Hit hit;
        if (pickAt(_lastMousePos.cast<float>(), hit)) {
            _IK_target = hit.intersectionPoint();
        }

    } else {
        // trackball
        if (action == GLFW_PRESS) {
            _trackingMode = TM_ROTATE_AROUND;
            _trackball.start();
            _trackball.track(_lastMousePos);
        } else if (action == GLFW_RELEASE) {
            _trackingMode = TM_NO_TRACK;
        }
    }
}

/*!
   callback to manage mouse : called when user move mouse with button pressed
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::mouseMoved(int x, int y)
{
    if (_trackingMode == TM_ROTATE_AROUND) {
        _trackball.track(Vector2i(x, y));
    }

    _lastMousePos = Vector2i(x, y);
}

void Viewer::mouseScroll(double /*x*/, double y)
{
    _cam.zoom(-0.1f * float(y));
}

/*!
   callback to manage keyboard interactions
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::keyPressed(int key, int action, int /*mods*/)
{
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_R) {
            loadShaders();
        } else if (key == GLFW_KEY_W) {
            _wireframe = !_wireframe;
        }
    }

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_UP) {
        } else if (key == GLFW_KEY_DOWN) {
        } else if (key == GLFW_KEY_LEFT) {
        } else if (key == GLFW_KEY_RIGHT) {
        } else if (key == GLFW_KEY_PAGE_UP) {
        } else if (key == GLFW_KEY_PAGE_DOWN) {
        }
    }
}

void Viewer::charPressed(int /*key*/)
{
}
