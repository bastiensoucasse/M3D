#include "viewer.h"

#include "camera.h"

using namespace Eigen;

Viewer::Viewer()
    : _winWidth(0)
    , _winHeight(0)
{
}

Viewer::~Viewer()
{
}

void Viewer::init(int w, int h)
{
    glClearColor(0, 0, 0, 0);

    loadShaders();

    if (!_mesh.load(DATA_DIR "/models/sphere.obj"))
        exit(1);

    _mesh.initVBA();

    reshape(w, h);

    _cam.setPerspective(M_PI / 3, .1, 20000);
    _cam.lookAt(Vector3f(0, 0, 4), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
    _trackball.setCamera(&_cam);

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
    glViewport(0, 0, _winWidth, _winHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shader.activate();

    Matrix4f transform_matrix = Matrix4f::Identity();
    glUniformMatrix4fv(_shader.getUniformLocation("transform_matrix"), 1, GL_FALSE, transform_matrix.data());

    Matrix4f view_matrix = _cam.viewMatrix();
    glUniformMatrix4fv(_shader.getUniformLocation("view_matrix"), 1, GL_FALSE, view_matrix.data());

    Matrix4f projection_matrix = _cam.projectionMatrix();
    glUniformMatrix4fv(_shader.getUniformLocation("projection_matrix"), 1, GL_FALSE, projection_matrix.data());

    Matrix3f local_to_camera_matrix = (view_matrix * transform_matrix).topLeftCorner(3, 3);

    Matrix3f normal_matrix = local_to_camera_matrix.inverse().transpose();
    glUniformMatrix3fv(_shader.getUniformLocation("normal_matrix"), 1, GL_FALSE, normal_matrix.data());

    Vector3f light_direction = (local_to_camera_matrix * Vector3f(1, 0, 1).normalized()).normalized();
    glUniform3fv(_shader.getUniformLocation("light_direction"), 1, light_direction.data());

    Vector3f light_color = Vector3f(1, 1, 1);
    glUniform3fv(_shader.getUniformLocation("light_color"), 1, light_color.data());

    _mesh.draw(_shader);

    _shader.deactivate();
}

void Viewer::updateAndDrawScene()
{
    drawScene();
}

void Viewer::loadShaders()
{
    _shader.loadFromFiles(DATA_DIR "/shaders/simple.vert", DATA_DIR "/shaders/simple.frag");
    checkError();
}

void Viewer::keyPressed(int key, int action, int /* mods */)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        loadShaders();

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_UP)
            _offset.y() += .05;
        else if (key == GLFW_KEY_DOWN)
            _offset.y() -= .05;
        else if (key == GLFW_KEY_LEFT)
            _offset.x() -= .05;
        else if (key == GLFW_KEY_RIGHT)
            _offset.x() += .05;
        else if (key == GLFW_KEY_PAGE_UP)
            _zoom *= 1.1;
        else if (key == GLFW_KEY_PAGE_DOWN)
            _zoom /= 1.1;
    }
}

void Viewer::mousePressed(GLFWwindow* /* window */, int /* button */, int action)
{
    if (action == GLFW_PRESS) {
        _trackingMode = TM_ROTATE_AROUND;
        _trackball.start();
        _trackball.track(_lastMousePos);
    } else if (action == GLFW_RELEASE)
        _trackingMode = TM_NO_TRACK;
}

void Viewer::mouseMoved(int x, int y)
{
    if (_trackingMode == TM_ROTATE_AROUND)
        _trackball.track(Vector2i(x, y));

    _lastMousePos = Vector2i(x, y);
}

void Viewer::mouseScroll(double /* x */, double y)
{
    _cam.zoom(-.1 * y);
}

void Viewer::charPressed(int /* key */)
{
}
