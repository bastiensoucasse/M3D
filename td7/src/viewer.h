#ifndef VIEWER_H
#define VIEWER_H

#include "camera.h"
#include "mesh.h"
#include "opengl.h"
#include "shader.h"
#include "trackball.h"

#include <iostream>

class Viewer {
public:
    Viewer();
    virtual ~Viewer();

    void init(int w, int h);
    void drawScene();
    void updateAndDrawScene();
    void reshape(int w, int h);
    void loadShaders();

    void mousePressed(GLFWwindow* window, int button, int action);
    void mouseMoved(int x, int y);
    void mouseScroll(double x, double y);
    void keyPressed(int key, int action, int mods);
    void charPressed(int key);

private:
    int _winWidth, _winHeight;

    Camera _cam;
    Shader _shader;
    Mesh _mesh;

    float _theta;
    bool _rotate;

    GLuint _texId0, _texId1, _texId2;

    // GLuint _samplerId;
    // GLenum _minFilter = GL_NEAREST;
    // GLenum _magFilter = GL_NEAREST;

    GLuint _samplerId;
    GLenum _minFilters[6] = { GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR };
    GLenum _magFilters[2] = { GL_NEAREST, GL_LINEAR };
    int _minFilter = 0;
    int _magFilter = 0;

    enum TrackMode {
        TM_NO_TRACK = 0,
        TM_ROTATE_AROUND,
        TM_ZOOM,
        TM_LOCAL_ROTATE,
        TM_FLY_Z,
        TM_FLY_PAN
    };

    TrackMode _trackingMode = TM_NO_TRACK;
    Trackball _trackball;
    Eigen::Vector2i _lastMousePos;
};

#endif
