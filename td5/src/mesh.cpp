#include "mesh.h"

#include <Eigen/Geometry>
#include <ObjFormat/ObjFormat.h>
#include <fstream>
#include <iostream>
#include <limits>

#include "shader.h"

using namespace Eigen;

bool Mesh::load(const std::string& filename)
{
    std::string ext = filename.substr(filename.size() - 3, 3);
    if (ext == "off" || ext == "OFF")
        return loadOFF(filename);
    else if (ext == "obj" || ext == "OBJ")
        return loadOBJ(filename);

    std::cerr << "Mesh: extension \'" << ext << "\' not supported." << std::endl;
    return false;
}

void Mesh::computeNormals()
{
    for (int i = 0; i < mVertices.size(); i++)
        mVertices.at(i).normal.setZero();

    for (int i = 0; i < mFaces.size(); i++) {
        Vertex& A = mVertices.at(mFaces.at(i).x());
        Vertex& B = mVertices.at(mFaces.at(i).y());
        Vertex& C = mVertices.at(mFaces.at(i).z());

        Vector3f normal = (B.position - A.position).cross(C.position - A.position);

        A.normal += normal;
        B.normal += normal;
        C.normal += normal;
    }

    for (int i = 0; i < mVertices.size(); i++)
        mVertices.at(i).normal.normalize();
}

void Mesh::initVBA()
{
    glGenBuffers(1, &mVertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mVertices.size(), mVertices[0].position.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mIndexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3i) * mFaces.size(), mFaces[0].data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &mVertexArrayId);

    mIsInitialized = true;
}

Mesh::~Mesh()
{
    if (mIsInitialized) {
        glDeleteBuffers(1, &mVertexBufferId);
        glDeleteBuffers(1, &mIndexBufferId);
    }
}

void Mesh::draw(const Shader& shd)
{
    glBindVertexArray(mVertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);

    int vertex_position = shd.getAttribLocation("vertex_position");
    if (vertex_position >= 0) {
        glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 * sizeof(Vector3f)));
        glEnableVertexAttribArray(vertex_position);
    }

    int vertex_normal = shd.getAttribLocation("vertex_normal");
    if (vertex_normal >= 0) {
        glVertexAttribPointer(vertex_normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(1 * sizeof(Vector3f)));
        glEnableVertexAttribArray(vertex_normal);
    }

    int vertex_color = shd.getAttribLocation("vertex_color");
    if (vertex_color >= 0) {
        glVertexAttribPointer(vertex_color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vector3f)));
        glEnableVertexAttribArray(vertex_color);
    }

    glDrawElements(GL_TRIANGLES, 3 * mFaces.size(), GL_UNSIGNED_INT, 0);

    if (vertex_position >= 0)
        glDisableVertexAttribArray(vertex_position);

    if (vertex_normal >= 0)
        glDisableVertexAttribArray(vertex_normal);

    if (vertex_color >= 0)
        glDisableVertexAttribArray(vertex_color);

    checkError();
}

bool Mesh::loadOFF(const std::string& filename)
{
    std::ifstream in(filename.c_str(), std::ios::in);
    if (!in) {
        std::cerr << "File not found " << filename << std::endl;
        return false;
    }

    std::string header;
    in >> header;

    bool hasColor = false;

    if (header != "OFF") {
        if (header != "COFF") {
            std::cerr << "Wrong header = " << header << std::endl;
            return false;
        }
        hasColor = true;
    }

    int nofVertices, nofFaces, inull;
    int nb, id0, id1, id2;
    Vector3f v;

    in >> nofVertices >> nofFaces >> inull;

    for (int i = 0; i < nofVertices; ++i) {
        in >> v[0] >> v[1] >> v[2];
        mVertices.push_back(Vertex(v));

        if (hasColor) {
            Vector4f c;
            in >> c[0] >> c[1] >> c[2] >> c[3];
            mVertices[i].color = c / 255.;
        }
    }

    for (int i = 0; i < nofFaces; ++i) {
        in >> nb >> id0 >> id1 >> id2;
        assert(nb == 3);
        mFaces.push_back(Vector3i(id0, id1, id2));
    }

    in.close();

    computeNormals();
    return true;
}

bool Mesh::loadOBJ(const std::string& filename)
{
    ObjMesh* pRawObjMesh = ObjMesh::LoadFromFile(filename);

    if (!pRawObjMesh) {
        std::cerr << "Mesh::loadObj: error loading file " << filename << "." << std::endl;
        return false;
    }

    ObjMesh* pObjMesh = pRawObjMesh->createIndexedFaceSet(Obj::Options(Obj::AllAttribs | Obj::Triangulate));
    delete pRawObjMesh;
    pRawObjMesh = 0;

    mVertices.resize(pObjMesh->positions.size());

    for (std::size_t i = 0; i < pObjMesh->positions.size(); ++i) {
        mVertices[i] = Vertex(Vector3f(pObjMesh->positions[i].x, pObjMesh->positions[i].y, pObjMesh->positions[i].z));

        if (!pObjMesh->texcoords.empty())
            mVertices[i].texcoord = Vector2f(pObjMesh->texcoords[i]);

        if (!pObjMesh->normals.empty())
            mVertices[i].normal = Vector3f(pObjMesh->normals[i]);
    }

    for (std::size_t smi = 0; smi < pObjMesh->getNofSubMeshes(); ++smi) {
        const ObjSubMesh* pSrcSubMesh = pObjMesh->getSubMesh(int(smi));

        mFaces.reserve(pSrcSubMesh->getNofFaces());

        for (std::size_t fid = 0; fid < pSrcSubMesh->getNofFaces(); ++fid) {
            ObjConstFaceHandle srcFace = pSrcSubMesh->getFace(int(fid));
            mFaces.push_back(Vector3i(srcFace.vPositionId(0), srcFace.vPositionId(1), srcFace.vPositionId(2)));
        }
    }

    computeNormals();
    return true;
}
