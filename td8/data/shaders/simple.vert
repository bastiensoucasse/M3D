#version 330 core

#define M_PI 3.14159265358979323846264338327950288

uniform mat4 obj_mat;
uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat3 normal_mat;

in vec3 vtx_position;
in vec3 vtx_normal;
in vec2 vtx_texcoord;

out vec2 v_uv;
out vec3 v_normal;

vec3 cylinder(vec2 uv, vec3 A, vec3 B, float r)
{
    float u = uv.x;
    float v = uv.y * 2 * M_PI;

    // Base cylinder
    // return r * vec3(cos(v), sin(v), u);

    // Oriented cylinder
    // vec3 q = (1 - u) * A + u * B;
    // return q + r * vec3(cos(v), sin(v), 0);

    // Final cylinder
    vec3 t = normalize(B - A);
    vec3 b = normalize(cross(t, vec3(1, 1, 1)));
    vec3 n = normalize(cross(t, b));
    mat3 bnt = mat3(b, n, t);

    vec3 q = (1 - u) * A + u * B;
    return q + bnt * (r * vec3(cos(v), sin(v), 0));
}

void main()
{
    v_uv = vtx_texcoord;
    v_normal = normalize(normal_mat * vtx_normal);

    vec3 p = cylinder(vtx_texcoord, vec3(0, 0, 0), vec3(1, 0, 1), 1);
    gl_Position = proj_mat * (view_mat * (obj_mat * vec4(p, 1)));
}
