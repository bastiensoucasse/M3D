#include "viewer.h"

#include "camera.h"

using namespace Eigen;

Viewer::Viewer()
    : _winWidth(0)
    , _winHeight(0)
    , _zoom(1.)
{
}

Viewer::~Viewer()
{
}

void Viewer::init(int w, int h)
{
    loadShaders();

    if (!_mesh.load(DATA_DIR "/models/lemming.off"))
        exit(1);

    _mesh.initVBA();

    reshape(w, h);
    _trackball.setCamera(&_cam);

    glClearColor(.6, .6, .6, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
}

void Viewer::reshape(int w, int h)
{
    _winWidth = w;
    _winHeight = h;
    _cam.setViewport(w, h);
}

void Viewer::drawScene()
{
    // Define the tracing area
    glViewport(0, 0, _winWidth, _winHeight);

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Trace the scene
    _shader.activate();

    glUniform1i(_shader.getUniformLocation("mode"), 0);
    glUniform1f(_shader.getUniformLocation("zoom"), _zoom);
    glUniform2fv(_shader.getUniformLocation("translate"), 1, _translate.data());
    _mesh.draw(_shader);

    if (true)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDepthFunc(GL_LEQUAL);
        glUniform1i(_shader.getUniformLocation("mode"), 1);
        _mesh.draw(_shader);
        glDepthFunc(GL_LESS);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    _shader.deactivate();
}

void Viewer::updateAndDrawScene()
{
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
            _translate.y() += .1;
        } else if (key == GLFW_KEY_DOWN) {
            _translate.y() -= .1;
        } else if (key == GLFW_KEY_LEFT) {
            _translate.x() -= .1;
        } else if (key == GLFW_KEY_RIGHT) {
            _translate.x() += .1;
        } else if (key == GLFW_KEY_PAGE_UP) {
            _zoom *= 1.25;
        } else if (key == GLFW_KEY_PAGE_DOWN) {
            _zoom /= 1.25;
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
    } else if (action == GLFW_RELEASE)
        _trackingMode = TM_NO_TRACK;
}

/*!
   callback to manage mouse : called when user move mouse with button pressed
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::mouseMoved(int x, int y)
{
    if (_trackingMode == TM_ROTATE_AROUND)
        _trackball.track(Vector2i(x, y));

    _lastMousePos = Vector2i(x, y);
}

void Viewer::mouseScroll(double /*x*/, double y)
{
    _cam.zoom(-.1 * y);
}

void Viewer::charPressed(int /*key*/)
{
}
