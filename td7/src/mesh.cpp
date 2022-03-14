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

    if (ext == "obj" || ext == "OBJ")
        return loadOBJ(filename);

    std::cerr << "Mesh: extension \'" << ext << "\' not supported." << std::endl;
    return false;
}

void Mesh::computeNormals()
{
    for (size_t i = 0; i < mVertices.size(); i++) {
        Vertex& v = mVertices.at(i);

        v.normal.setZero();
        v.tangent.setZero();
        v.bitangent.setZero();
    }

    for (size_t i = 0; i < mFaces.size(); i++) {
        Vertex& a = mVertices.at(mFaces.at(i).x());
        Vertex& b = mVertices.at(mFaces.at(i).y());
        Vertex& c = mVertices.at(mFaces.at(i).z());

        const Vector3f q1 = b.position - a.position;
        const Vector3f q2 = c.position - a.position;

        const Vector3f normal = q1.cross(q2);

        const Vector2f st1 = b.texcoord - a.texcoord;
        const Vector2f st2 = c.texcoord - a.texcoord;

        const Vector3f tangent = Vector3f(st2.y() * q1.x() - st1.y() * q2.x(), st2.y() * q1.y() - st1.y() * q2.y(), st2.y() * q1.z() - st1.y() * q2.z()) / (st1.x() * st2.y() - st2.x() * st1.y());
        const Vector3f bitangent = Vector3f(st1.x() * q2.x() - st2.x() * q1.x(), st1.x() * q2.y() - st2.x() * q1.y(), st1.x() * q2.z() - st2.x() * q1.z()) / (st1.x() * st2.y() - st2.x() * st1.y());

        a.normal += normal, a.tangent += tangent, a.bitangent += bitangent;
        b.normal += normal, b.tangent += tangent, b.bitangent += bitangent;
        c.normal += normal, c.tangent += tangent, c.bitangent += bitangent;
    }

    for (size_t i = 0; i < mVertices.size(); i++) {
        Vertex& v = mVertices.at(i);

        v.normal.normalize();
        v.tangent.normalize();
        v.bitangent.normalize();

        v.tangent = v.tangent - v.normal.dot(v.tangent) * v.normal;
        v.bitangent = v.bitangent - v.normal.dot(v.bitangent) * v.normal - v.tangent.dot(v.bitangent) * v.tangent / v.tangent.norm();

        v.tangent.normalize();
        v.bitangent.normalize();
    }
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

    // For vec3 vert_position.
    int vert_position = shd.getAttribLocation("vert_position");
    if (vert_position >= 0) {
        glVertexAttribPointer(vert_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(vert_position);
    }

    // For vec3 vert_normal.
    int vert_normal = shd.getAttribLocation("vert_normal");
    if (vert_normal >= 0) {
        glVertexAttribPointer(vert_normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3f)));
        glEnableVertexAttribArray(vert_normal);
    }

    // For vec3 vert_tangent.
    int vert_tangent = shd.getAttribLocation("vert_tangent");
    if (vert_tangent >= 0) {
        glVertexAttribPointer(vert_tangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vector3f)));
        glEnableVertexAttribArray(vert_tangent);
    }

    // For vec3 vert_bitangent.
    int vert_bitangent = shd.getAttribLocation("vert_bitangent");
    if (vert_bitangent >= 0) {
        glVertexAttribPointer(vert_bitangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(Vector3f)));
        glEnableVertexAttribArray(vert_bitangent);
    }

    // For vec4 vert_color.
    int vert_color = shd.getAttribLocation("vert_color");
    if (vert_color >= 0) {
        glVertexAttribPointer(vert_color, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(4 * sizeof(Vector3f)));
        glEnableVertexAttribArray(vert_color);
    }

    // For vec2 vert_texcoords.
    int vert_texcoords = shd.getAttribLocation("vert_texcoords");
    if (vert_texcoords >= 0) {
        glVertexAttribPointer(vert_texcoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(4 * sizeof(Vector3f) + sizeof(Vector4f)));
        glEnableVertexAttribArray(vert_texcoords);
    }

    glDrawElements(GL_TRIANGLES, 3 * mFaces.size(), GL_UNSIGNED_INT, 0);

    if (vert_position >= 0)
        glDisableVertexAttribArray(vert_position);
    if (vert_normal >= 0)
        glDisableVertexAttribArray(vert_normal);
    if (vert_tangent >= 0)
        glDisableVertexAttribArray(vert_tangent);
    if (vert_bitangent >= 0)
        glDisableVertexAttribArray(vert_bitangent);
    if (vert_color >= 0)
        glDisableVertexAttribArray(vert_color);
    if (vert_texcoords >= 0)
        glDisableVertexAttribArray(vert_texcoords);

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

    if (pObjMesh->normals.empty())
        computeNormals();

    return true;
}
