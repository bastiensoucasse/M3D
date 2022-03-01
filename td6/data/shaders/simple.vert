#version 410 core

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_color;
in vec2 vertex_texcoords;

out vec3 var_view;
out vec3 var_normal;
out vec3 var_color;
out vec2 var_texcoords;

uniform mat4 transform_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat3 normal_matrix;

void main()
{
    vec4 local_position = vec4(vertex_position, 1);
    vec4 world_position = transform_matrix * local_position;
    vec4 camera_position = view_matrix * world_position;

    var_view = normalize(-camera_position.xyz);
    var_normal = normalize(normal_matrix * vertex_normal);
    var_color = vertex_color;
    var_texcoords = vertex_texcoords;

    gl_Position = projection_matrix * camera_position;
}
