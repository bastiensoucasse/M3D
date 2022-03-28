#include "mesh.h"

#include "shader.h"
#include <Eigen/Geometry>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;
using namespace Eigen;
using namespace surface_mesh;

void Mesh::subdivide()
{
    Surface_mesh new_mesh;

    // Retrieve original vertices positions.
    auto positions = _halfedge.get_vertex_property<Point>("v:point");

    // Add original vertices to the mesh, with updated positions.
    auto vertex_mapping = _halfedge.add_vertex_property<Surface_mesh::Vertex>("v:mapping");
    for (Surface_mesh::Vertex vertex : _halfedge.vertices()) {
        // Retrieve the vertex valence.
        int vertex_valence = _halfedge.valence(vertex);

        // Check if the vertex is boundary.
        bool vertex_boundary = _halfedge.is_boundary(vertex);
        if (vertex_boundary) {
            // Compute the vertex new position.
            Point vertex_position = 3. / 4. * positions[vertex];
            Surface_mesh::Halfedge vertex_halfedge = _halfedge.halfedge(vertex);
            for (int i = 0; i < vertex_valence; i++) {
                if (_halfedge.is_boundary(_halfedge.to_vertex(vertex_halfedge)))
                    vertex_position += 1. / 8. * positions[_halfedge.to_vertex(vertex_halfedge)];
                vertex_halfedge = _halfedge.opposite_halfedge(vertex_halfedge);
                vertex_halfedge = _halfedge.next_halfedge(vertex_halfedge);
            }

            // Add the updated original vertex to the new mesh.
            vertex_mapping[vertex] = new_mesh.add_vertex(vertex_position);
            continue;
        }

        // Retrieve the beta value.
        double beta = 0;
        if (vertex_valence == 3)
            beta = 3. / 16.;
        if (vertex_valence > 3)
            beta = 1. / (double)vertex_valence * (5. / 8. - pow((3. / 8.) + (1. / 4.) * cos(2 * M_PI / vertex_valence), 2));

        // Compute the vertex new position.
        Point vertex_position = (1. - beta * vertex_valence) * positions[vertex];
        Surface_mesh::Halfedge vertex_halfedge = _halfedge.halfedge(vertex);
        for (int i = 0; i < vertex_valence; i++) {
            vertex_position += beta * positions[_halfedge.to_vertex(vertex_halfedge)];
            vertex_halfedge = _halfedge.opposite_halfedge(vertex_halfedge);
            vertex_halfedge = _halfedge.next_halfedge(vertex_halfedge);
        }

        // Add the updated original vertex to the new mesh.
        vertex_mapping[vertex] = new_mesh.add_vertex(vertex_position);
    }

    // Add new vertices to the mesh.
    auto edge_mapping = _halfedge.add_edge_property<Surface_mesh::Vertex>("e:mapping");
    for (Surface_mesh::Edge edge : _halfedge.edges()) {
        // Retrieve the two edge vertices.
        Surface_mesh::Vertex edge_vertices[2];
        for (int i = 0; i < 2; i++)
            edge_vertices[i] = _halfedge.vertex(edge, i);

        // Check if the edge is boundary.
        bool edge_boundary = _halfedge.is_boundary(edge);
        const int num_faces = edge_boundary ? 1 : 2;

        // Retrieve the two edge faces.
        Surface_mesh::Face edge_faces[num_faces];
        for (int i = 0; i < num_faces; i++)
            edge_faces[i] = _halfedge.face(edge, i);

        // Retrieve the two edge opposite vertices (one by face).
        Surface_mesh::Vertex edge_opposite_vertices[num_faces];
        for (int i = 0; i < num_faces; i++) {
            // Retrieve the only edge face vertex which is not a vertex of the edge.
            Surface_mesh::Halfedge edge_face_halfedge = _halfedge.halfedge(edge_faces[i]);
            do {
                edge_opposite_vertices[i] = _halfedge.to_vertex(edge_face_halfedge);
                edge_face_halfedge = _halfedge.next_halfedge(edge_face_halfedge);
            } while (edge_opposite_vertices[i] == edge_vertices[0] || edge_opposite_vertices[i] == edge_vertices[1]);
        }

        // Add the new vertex to the mesh.
        Point new_vertex_position;
        if (edge_boundary)
            new_vertex_position = 1. / 2. * positions[edge_vertices[0]] + 1. / 2. * positions[edge_vertices[1]];
        else
            new_vertex_position = 3. / 8. * positions[edge_vertices[0]] + 3. / 8. * positions[edge_vertices[1]] + 1. / 8. * positions[edge_opposite_vertices[0]] + 1. / 8. * positions[edge_opposite_vertices[1]];
        edge_mapping[edge] = new_mesh.add_vertex(new_vertex_position);
    }

    // Add new faces to the mesh.
    for (Surface_mesh::Face face : _halfedge.faces()) {
        // Retrieve the face halfedge.
        Surface_mesh::Halfedge face_haldedge = _halfedge.halfedge(face);

        // Retrieve the 3 face vertices.
        Surface_mesh::Vertex face_vertices[3];
        for (int i = 0; i < 3; i++) {
            face_vertices[i] = _halfedge.to_vertex(face_haldedge);
            face_haldedge = _halfedge.next_halfedge(face_haldedge);
        }

        // Retrieve the 3 face edge vetices.
        Surface_mesh::Vertex face_edge_vertices[3];
        for (int i = 0; i < 3; i++) {
            face_edge_vertices[i] = edge_mapping[_halfedge.edge(face_haldedge)];
            face_haldedge = _halfedge.next_halfedge(face_haldedge);
        }

        // Add the 4 faces to the new mesh.
        new_mesh.add_triangle(face_edge_vertices[0], face_edge_vertices[1], face_edge_vertices[2]);
        new_mesh.add_triangle(face_edge_vertices[0], face_edge_vertices[2], face_vertices[2]);
        new_mesh.add_triangle(face_edge_vertices[2], face_edge_vertices[1], face_vertices[1]);
        new_mesh.add_triangle(face_edge_vertices[1], face_edge_vertices[0], face_vertices[0]);
    }

    // Update mesh.
    new_mesh.update_face_normals();
    new_mesh.update_vertex_normals();
    _halfedge = new_mesh;
    updateHalfedgeToMesh();
    updateVBO();
}

Mesh::~Mesh()
{
    if (_isInitialized) {
        glDeleteBuffers(1, &_vertexBufferId);
        glDeleteBuffers(1, &_indexBufferId);
        glDeleteVertexArrays(1, &_vertexArrayId);
    }
}

bool Mesh::load(const std::string& filename)
{
    std::cout << "Loading: " << filename << std::endl;

    if (!_halfedge.read(filename))
        return false;

    _halfedge.update_face_normals();
    _halfedge.update_vertex_normals();

    updateHalfedgeToMesh();

    return true;
}

void Mesh::updateHalfedgeToMesh()
{
    // vertex properties
    Surface_mesh::Vertex_property<Point> vertices = _halfedge.get_vertex_property<Point>("v:point");
    Surface_mesh::Vertex_property<Point> vnormals = _halfedge.get_vertex_property<Point>("v:normal");
    Surface_mesh::Vertex_property<Texture_coordinate> texcoords = _halfedge.get_vertex_property<Texture_coordinate>("v:texcoord");
    Surface_mesh::Vertex_property<Color> colors = _halfedge.get_vertex_property<Color>("v:color");

    // vertex iterator
    Surface_mesh::Vertex_iterator vit;

    Vector3f pos;
    Vector3f normal;
    Vector2f tex;
    Vector4f color;
    _vertices.clear();
    for (vit = _halfedge.vertices_begin(); vit != _halfedge.vertices_end(); ++vit) {
        pos = vertices[*vit];
        normal = vnormals[*vit];
        if (texcoords)
            tex = texcoords[*vit];
        if (colors)
            color << colors[*vit], 1.0f;
        else
            color = Vector4f(0.6f, 0.6f, 0.6f, 1.0f);

        _vertices.push_back(Vertex(pos, normal, color, tex));
    }

    // face iterator
    Surface_mesh::Face_iterator fit, fend = _halfedge.faces_end();
    // vertex circulator
    Surface_mesh::Vertex_around_face_circulator fvit, fvend;
    Surface_mesh::Vertex v0, v1, v2;
    _faces.clear();
    for (fit = _halfedge.faces_begin(); fit != fend; ++fit) {
        fvit = fvend = _halfedge.vertices(*fit);
        v0 = *fvit;
        ++fvit;
        v2 = *fvit;

        do {
            v1 = v2;
            ++fvit;
            v2 = *fvit;
            _faces.push_back(Vector3i(v0.idx(), v1.idx(), v2.idx()));
        } while (++fvit != fvend);
    }

    // updateNormals();
}

void Mesh::init()
{
    glGenVertexArrays(1, &_vertexArrayId);
    glGenBuffers(1, &_vertexBufferId);
    glGenBuffers(1, &_indexBufferId);

    updateVBO();

    _isInitialized = true;
}

void Mesh::updateNormals()
{
    // pass 1: set the normal to 0
    for (std::vector<Vertex>::iterator v_iter = _vertices.begin(); v_iter != _vertices.end(); ++v_iter)
        v_iter->normal.setZero();

    // pass 2: compute face normals and accumulate
    for (std::size_t j = 0; j < _faces.size(); ++j) {
        Vector3f v0 = _vertices[_faces[j][0]].position;
        Vector3f v1 = _vertices[_faces[j][1]].position;
        Vector3f v2 = _vertices[_faces[j][2]].position;

        Vector3f n = (v1 - v0).cross(v2 - v0).normalized();

        _vertices[_faces[j][0]].normal += n;
        _vertices[_faces[j][1]].normal += n;
        _vertices[_faces[j][2]].normal += n;
    }

    // pass 3: normalize
    for (std::vector<Vertex>::iterator v_iter = _vertices.begin(); v_iter != _vertices.end(); ++v_iter)
        v_iter->normal.normalize();
}

void Mesh::updateVBO()
{
    glBindVertexArray(_vertexArrayId);

    // activate the VBO:
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
    // copy the data from host's RAM to GPU's video memory:
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), _vertices[0].position.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3i) * _faces.size(), _faces[0].data(), GL_STATIC_DRAW);
}

void Mesh::draw(const Shader& shd)
{
    if (!_isInitialized)
        init();

    // Activate the VBO of the current mesh:
    glBindVertexArray(_vertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);

    // Specify vertex data

    // 1 - get id of the attribute "vtx_position" as declared as "in vec3 vtx_position" in the vertex shader
    int vertex_loc = shd.getAttribLocation("vtx_position");
    if (vertex_loc >= 0) {
        // 2 - tells OpenGL where to find the x, y, and z coefficients:
        glVertexAttribPointer(vertex_loc, // id of the attribute
            3, // number of coefficients (here 3 for x, y, z)
            GL_FLOAT, // type of the coefficients (here float)
            GL_FALSE, // for fixed-point number types only
            sizeof(Vertex), // number of bytes between the x coefficient of two vertices
                            // (e.g. number of bytes between x_0 and x_1)
            0); // number of bytes to get x_0
        // 3 - activate this stream of vertex attribute
        glEnableVertexAttribArray(vertex_loc);
    }

    int normal_loc = shd.getAttribLocation("vtx_normal");
    if (normal_loc >= 0) {
        glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3f));
        glEnableVertexAttribArray(normal_loc);
    }

    int color_loc = shd.getAttribLocation("vtx_color");
    if (color_loc >= 0) {
        glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vector3f)));
        glEnableVertexAttribArray(color_loc);
    }

    int texcoord_loc = shd.getAttribLocation("vtx_texcoord");
    if (texcoord_loc >= 0) {
        glVertexAttribPointer(texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vector3f) + sizeof(Vector4f)));
        glEnableVertexAttribArray(texcoord_loc);
    }

    // send the geometry
    glDrawElements(GL_TRIANGLES, (gl::GLsizei)(3 * _faces.size()), GL_UNSIGNED_INT, 0);

    // at this point the mesh has been drawn and raserized into the framebuffer!
    if (vertex_loc >= 0)
        glDisableVertexAttribArray(vertex_loc);
    if (normal_loc >= 0)
        glDisableVertexAttribArray(normal_loc);
    if (color_loc >= 0)
        glDisableVertexAttribArray(color_loc);
    if (texcoord_loc >= 0)
        glDisableVertexAttribArray(texcoord_loc);

    checkError();
}
