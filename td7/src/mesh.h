#ifndef MESH_H
#define MESH_H

#include <Eigen/Core>
#include <string>
#include <vector>

class Shader;

class Mesh {
    typedef Eigen::Matrix<float, 2, 1, Eigen::DontAlign> Vector2f;
    typedef Eigen::Vector3f Vector3f;
    typedef Eigen::Matrix<float, 4, 1, Eigen::DontAlign> Vector4f;
    typedef Eigen::Vector3i Vector3i;

public:
    Mesh() { }
    virtual ~Mesh();
    bool load(const std::string& filename);
    void computeNormals();
    void initVBA();
    virtual void draw(const Shader& shd);

protected:
    bool loadOFF(const std::string& filename);
    bool loadOBJ(const std::string& filename);

    struct Vertex {
        Vertex(
            const Vector3f& pos = Vector3f::Zero(),
            const Vector3f& n = Vector3f::Zero(),
            const Vector3f& t = Vector3f::Zero(),
            const Vector3f& b = Vector3f::Zero())
            : position(pos)
            , normal(n)
            , tangent(t)
            , bitangent(b)
            , color(Vector4f(.6f, .6f, .6f, 1.f))
            , texcoord(Vector2f::Zero())
        {
        }

        Vector3f position;
        Vector3f normal;
        Vector3f tangent;
        Vector3f bitangent;
        Vector4f color;
        Vector2f texcoord;
    };

    std::vector<Vertex> mVertices;

    std::vector<Vector3i> mFaces;

    unsigned int mVertexArrayId;
    unsigned int mVertexBufferId;
    unsigned int mIndexBufferId;
    bool mIsInitialized;
};

#endif // MESH_H
