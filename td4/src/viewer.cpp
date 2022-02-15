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

////////////////////////////////////////////////////////////////////////////////
// GL stuff

// initialize OpenGL context
void Viewer::init(int w, int h)
{
    loadShaders();

    // if(!_mesh.load(DATA_DIR"/models/quad.off"))
    //     exit(1);
    // if (!_mesh.load(DATA_DIR "/models/chair.off"))
    //     exit(1);
    if (!_mesh.load(DATA_DIR "/models/lemming.off"))
        exit(1);

    _mesh.initVBA();

    reshape(w, h);

    _cam.setPerspective(M_PI / 3, .1f, 20000.f);
    _cam.lookAt(
        Vector3f(-2.f, 2, -2.f),
        Vector3f(0.f, 0.f, 0.f),
        Vector3f(0.f, 1.f, 0.f)
    );
    _trackball.setCamera(&_cam);

    glClearColor(0.5, 0.5, 0.5, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
}

void Viewer::reshape(int w, int h)
{
    _winWidth = w;
    _winHeight = h;
    _cam.setViewport(w, h);
}

/*!
   callback to draw graphic primitives
 */
void Viewer::drawScene()
{
    glViewport(0, 0, _winWidth, _winHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shader.activate();

    glUniform1i(_shader.getUniformLocation("wireframe"), 0);

    Matrix4f transform;
    transform.setIdentity();
    glUniformMatrix4fv(_shader.getUniformLocation("transform"), 1, GL_FALSE, transform.data());

    Matrix4f view = _cam.viewMatrix();
    glUniformMatrix4fv(_shader.getUniformLocation("view"), 1, GL_FALSE, view.data());

    Matrix4f projection = _cam.projectionMatrix();
    glUniformMatrix4fv(_shader.getUniformLocation("projection"), 1, GL_FALSE, projection.data());

    _mesh.draw(_shader);

    if (_wireframe) {
        glUniform1i(_shader.getUniformLocation("wireframe"), 1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDepthFunc(GL_LEQUAL);

        _mesh.draw(_shader);

        glUniform1i(_shader.getUniformLocation("wireframe"), 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDepthFunc(GL_LESS);
    }

    _shader.deactivate();
}

void Viewer::drawScene2D()
{
    // Matrix4f M;
    // M <<  _zoom, 0, 0, _offset.x(),
    //       0, _zoom, 0, _offset.y(),
    //       0, 0, 1, 0,
    //       0, 0, 0, 1;

    glViewport(0, 0, _winWidth, _winHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _shader.activate();

    // 1st chair
    Affine3f A1;
    A1 = Translation3f(-.5f, -1.f, 0.f) * Scaling(.5f, .5f, 1.f);
    glUniformMatrix4fv(_shader.getUniformLocation("transform"), 1, GL_FALSE, A1.data());
    _mesh.draw(_shader);

    // 2nd chair
    Affine3f A2;
    A2 = Translation3f(.5f, -1.f, 0.f) * Scaling(-.5f, .5f, 1.f);
    glUniformMatrix4fv(_shader.getUniformLocation("transform"), 1, GL_FALSE, A2.data());
    _mesh.draw(_shader);

    // 3rd chair
    Affine3f A3;
    Vector3f center(0.f, .5f, 0.f);
    A3 = Translation3f(center) * AngleAxisf(_zoom * M_PI, Vector3f::UnitZ()) * Translation3f(-center);
    glUniformMatrix4fv(_shader.getUniformLocation("transform"), 1, GL_FALSE, A3.data());
    _mesh.draw(_shader);

    _shader.deactivate();
}

void Viewer::updateAndDrawScene()
{
    // drawScene2D();
    drawScene();
}

void Viewer::loadShaders()
{
    // Here we can load as many shaders as we want, currently we have only one:
    _shader.loadFromFiles(DATA_DIR "/shaders/simple.vert", DATA_DIR "/shaders/simple.frag");
    checkError();
}

////////////////////////////////////////////////////////////////////////////////
// Events

/*!
   callback to manage keyboard interactions
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::keyPressed(int key, int action, int /*mods*/)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        loadShaders();
    }

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_UP) {
            _offset.y() += 0.05f;
        } else if (key == GLFW_KEY_DOWN) {
            _offset.y() -= 0.05f;
        } else if (key == GLFW_KEY_LEFT) {
            _offset.x() -= 0.05f;
        } else if (key == GLFW_KEY_RIGHT) {
            _offset.x() += 0.05f;
        } else if (key == GLFW_KEY_PAGE_UP) {
            _zoom *= 1.1f;
        } else if (key == GLFW_KEY_PAGE_DOWN) {
            _zoom /= 1.1f;
        } else if (key = GLFW_KEY_W) {
            _wireframe = !_wireframe;
        }
    }
}

/*!
   callback to manage mouse : called when user press or release mouse button
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::mousePressed(GLFWwindow* /*window*/, int /*button*/, int action)
{
    if (action == GLFW_PRESS) {
        _trackingMode = TM_ROTATE_AROUND;
        _trackball.start();
        _trackball.track(_lastMousePos);
    } else if (action == GLFW_RELEASE) {
        _trackingMode = TM_NO_TRACK;
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
    _cam.zoom(-0.1f * y);
}

void Viewer::charPressed(int /*key*/)
{
}
