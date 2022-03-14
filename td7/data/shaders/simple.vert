#version 410 core

in vec3 vert_position;
in vec3 vert_normal;
in vec3 vert_tangent;
in vec3 vert_bitangent;
in vec4 vert_color;
in vec2 vert_texcoords;

out vec3 frag_view;
out vec3 frag_tangent;
out vec3 frag_bitangent;
out vec3 frag_normal;
out vec4 frag_color;
out vec2 frag_texcoords;

uniform mat4 transform_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    frag_view = normalize(-(view_matrix * (transform_matrix * vec4(vert_position, 1))).xyz);
    frag_tangent = normalize((view_matrix * (transform_matrix * vec4(vert_tangent, 1))).xyz);
    frag_bitangent = normalize((view_matrix * (transform_matrix * vec4(vert_bitangent, 1))).xyz);
    frag_normal = normalize((view_matrix * (transform_matrix * vec4(vert_normal, 1))).xyz);
    frag_color = vert_color;
    frag_texcoords = vert_texcoords;

    gl_Position = projection_matrix * (view_matrix * (transform_matrix * vec4(vert_position, 1)));
}
