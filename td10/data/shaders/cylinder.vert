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

out vec3 v_normal;
out vec3 v_view;
out vec2 v_uv;

vec3 cylinder(vec2 uv, float r, float l) {
    return (M[1] * vec4(r * cos(uv.x * 2 * M_PI), r * sin(uv.x * 2 * M_PI), l * uv.y, 1)).xyz;
}

void main() {
    v_uv = vtx_texcoord;

    vec3 pos = cylinder(v_uv, .5, L.x + L.y + L.z);
    vec3 normal = vec3(pos.x, pos.y, 0);

    v_normal = normalize(normal_mat * normal);
    vec4 p = view_mat * (obj_mat * vec4(pos, 1.));
    v_view = normalize(-p.xyz);

    gl_Position = proj_mat * p;
}
