#version 410 core

#define M_PI 3.14159265358979323846264338327950288

uniform mat4 obj_mat;
uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat3 normal_mat;
uniform vec3 L;
uniform mat4 M[3];

in vec3 vtx_position;
in vec3 vtx_normal;
in vec2 vtx_texcoord;

out vec3 v_view;
out vec3 v_normal;
out vec2 v_uv;

vec4 cylinder(vec2 uv, float r, float l) {
    float z_pos = l * uv.y;

    if (z_pos < 0 || z_pos > L.x + L.y + L.z)
        return vec4(0, 0, 0, 0);

    int i = 0;
    if (z_pos > L.x)
        i = 1;
    if (z_pos > L.x + L.y)
        i = 2;

    return M[i] * vec4(r * cos(uv.x * 2 * M_PI), r * sin(uv.x * 2 * M_PI), z_pos, 1);
}

void main() {
    vec4 local_position = cylinder(vtx_texcoord, 0.5, L.x + L.y + L.z);
    vec4 world_position = obj_mat * local_position;
    vec4 camera_position = view_mat * world_position;
    vec3 normal = vec3(local_position.x, local_position.y, 0);

    v_view = normalize(-camera_position.xyz);
    v_normal = normalize(normal_mat * normal);
    v_uv = vtx_texcoord;

    gl_Position = proj_mat * camera_position;
}
