#include "viewer.h"

#include "SOIL2.h"
#include "camera.h"

using namespace Eigen;

Viewer::Viewer()
    : _winWidth(0)
    , _winHeight(0)
    , _theta(0.f)
    , _rotate(false)
{
}

Viewer::~Viewer()
{
}

void Viewer::init(int w, int h)
{
    glClearColor(0.f, 0.f, 0.f, 0.f);
    loadShaders();

    // if (!_mesh.load(DATA_DIR "/models/earth.obj"))
    //     exit(1);
    // if (!_mesh.load(DATA_DIR "/models/plane.obj"))
    //     exit(1);
    if (!_mesh.load(DATA_DIR "/models/cow.obj"))
        exit(1);

    _mesh.initVBA();

    reshape(w, h);
    _cam.setPerspective(M_PI / 3, .1f, 20000.f);
    _cam.lookAt(Vector3f(-4.f, .5f, 4.f), Vector3f(0.f, 0.f, 0.f), Vector3f(0.f, 1.f, 0.f));
    _trackball.setCamera(&_cam);

    // _texId0 = SOIL_load_OGL_texture(DATA_DIR "/textures/earth.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
    // _texId1 = SOIL_load_OGL_texture(DATA_DIR "/textures/earth_clouds.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
    // _texId2 = SOIL_load_OGL_texture(DATA_DIR "/textures/earth_night.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);

    // _texId0 = SOIL_load_OGL_texture(DATA_DIR "/textures/checkerboard.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
    _texId0 = SOIL_load_OGL_texture(DATA_DIR "/textures/cow.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

    int texW = 256;
    int texH = 256;
    unsigned char colors[6][3] = { { 255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }, { 255, 255, 0 }, { 0, 255, 255 }, { 255, 0, 255 } };
    unsigned char data[texW * texH * 3];
    glGenTextures(1, &_texId0);
    glBindTexture(GL_TEXTURE_2D, _texId0);
    for (int l = 0;; ++l) {
        for (int i = 0; i < texW * texH; ++i) {
            data[3 * i] = colors[l][0];
            data[3 * i + 1] = colors[l][1];
            data[3 * i + 2] = colors[l][2];
        }
        glTexImage2D(GL_TEXTURE_2D, l, (GLint)GL_RGB, texW, texH, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        if (texW == 1 && texH == 1)
            break;
        texW = std::max(1, texW / 2);
        texH = std::max(1, texH / 2);
    }

    glGenSamplers(1, &_samplerId);

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

    Matrix4f transform_matrix = Affine3f(AngleAxisf(_theta, Vector3f(0.f, 1.f, 0.f))).matrix();
    glUniformMatrix4fv(_shader.getUniformLocation("transform_matrix"), 1, GL_FALSE, transform_matrix.data());

    Matrix4f view_matrix = _cam.viewMatrix();
    glUniformMatrix4fv(_shader.getUniformLocation("view_matrix"), 1, GL_FALSE, view_matrix.data());

    Matrix4f projection_matrix = _cam.projectionMatrix();
    glUniformMatrix4fv(_shader.getUniformLocation("projection_matrix"), 1, GL_FALSE, projection_matrix.data());

    Matrix3f local_to_camera_matrix = (view_matrix * transform_matrix).topLeftCorner(3, 3);

    Matrix3f normal_matrix = local_to_camera_matrix.inverse().transpose();
    glUniformMatrix3fv(_shader.getUniformLocation("normal_matrix"), 1, GL_FALSE, normal_matrix.data());

    Vector3f light_direction = (local_to_camera_matrix * Vector3f(1, 1, 1).normalized()).normalized();
    glUniform3fv(_shader.getUniformLocation("light_direction"), 1, light_direction.data());

    Vector3f light_color = Vector3f(1, 1, 1);
    glUniform3fv(_shader.getUniformLocation("light_color"), 1, light_color.data());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texId0);
    glUniform1i(_shader.getUniformLocation("sampler0"), 0);

    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, _texId1);
    // glUniform1i(_shader.getUniformLocation("sampler1"), 1);

    // glActiveTexture(GL_TEXTURE2);
    // glBindTexture(GL_TEXTURE_2D, _texId2);
    // glUniform1i(_shader.getUniformLocation("sampler2"), 2);

    glBindSampler(0, _samplerId);
    // glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, _minFilter);
    // glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, _magFilter);
    glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, GLint(_minFilters[_minFilter]));
    glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GLint(_magFilters[_magFilter]));

    _mesh.draw(_shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, 0);

    // glActiveTexture(GL_TEXTURE2);
    // glBindTexture(GL_TEXTURE_2D, 0);

    _shader.deactivate();
}

void Viewer::updateAndDrawScene()
{
    if (_rotate)
        _theta += 0.01 * M_PI;
    drawScene();
}

void Viewer::loadShaders()
{
    _shader.loadFromFiles(DATA_DIR "/shaders/simple.vert", DATA_DIR "/shaders/simple.frag");
    checkError();
}

void Viewer::mousePressed(GLFWwindow* /* window */, int /* button */, int action)
{
    if (action == GLFW_PRESS) {
        _trackingMode = TM_ROTATE_AROUND;
        _trackball.start();
        _trackball.track(_lastMousePos);
    } else if (action == GLFW_RELEASE) {
        _trackingMode = TM_NO_TRACK;
    }
}

void Viewer::mouseMoved(int x, int y)
{
    if (_trackingMode == TM_ROTATE_AROUND) {
        _trackball.track(Vector2i(x, y));
    }

    _lastMousePos = Vector2i(x, y);
}

void Viewer::mouseScroll(double /* x */, double y)
{
    _cam.zoom(-0.1 * y);
}

void Viewer::keyPressed(int key, int action, int /* mods */)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        loadShaders();
    else if (key == GLFW_KEY_A && action == GLFW_PRESS)
        _rotate = !_rotate;

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_UP)
            _minFilter = (_minFilter + 1) % 6;
        else if (key == GLFW_KEY_DOWN)
            _minFilter = abs(_minFilter - 1) % 6;
        else if (key == GLFW_KEY_LEFT)
            _magFilter = abs(_magFilter - 1) % 2;
        else if (key == GLFW_KEY_RIGHT)
            _magFilter = (_magFilter + 1) % 2;
    }
}

void Viewer::charPressed(int /* key */)
{
}
