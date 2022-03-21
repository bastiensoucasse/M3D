#version 330 core

#define M_PI 3.14159265358979323846264338327950288

#define B_0 vec3[4](vec3(-1, 0, 2), vec3(-0.3, 0, 4), vec3(0.3, 0, 2), vec3(1, 0, -0.5))
#define B_1 vec3[4](vec3(-0.5, -1, -1), vec3(1.5, 1, -0.3), vec3(-1.5, 1, 0.3), vec3(0.5, -1, 1))
#define B_2 vec3[4](vec3(-1, -0.5, -1), vec3(-1, 1, -0.3), vec3(1, -1, 0.3), vec3(1, 0.5, 1))

uniform mat4 obj_mat;
uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat3 normal_mat;
uniform float r_coef;

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

// vec3 bezier(float u, vec3 B[4], out mat3 F)
// {
//     float p0 = 1 - 3 * u + 3 * pow(u, 2) - pow(u, 3);
//     float p1 = 3 * u - 6 * pow(u, 2) + 3 * pow(u, 3);
//     float p2 = 3 * pow(u, 2) - 3 * pow(u, 3);
//     float p3 = pow(u, 3);
//     vec3 p = p0 * B[0] + p1 * B[1] + p2 * B[2] + p3 * B[3];

//     float dp0 = -3 + 6 * u - 3 * pow(u, 2);
//     float dp1 = 3 - 12 * u + 9 * pow(u, 2);
//     float dp2 = 6 * u - 9 * pow(u, 2);
//     float dp3 = 3 * pow(u, 2);
//     vec3 dp = dp0 * B[0] + dp1 * B[1] + dp2 * B[2] + dp3 * B[3];

//     float d2p0 = 6 - 6 * u;
//     float d2p1 = -12 + 18 * u;
//     float d2p2 = 6 - 18 * u;
//     float d2p3 = 6 * u;
//     vec3 d2p = d2p0 * B[0] + d2p1 * B[1] + d2p2 * B[2] + d2p3 * B[3];

//     float d3p0 = -6;
//     float d3p1 = 18;
//     float d3p2 = -18;
//     float d3p3 = 6;
//     vec3 d3p = d3p0 * B[0] + d3p1 * B[1] + d3p2 * B[2] + d3p3 * B[3];

//     vec3 t = normalize(dp);
//     vec3 b = normalize(cross(dp, d2p));
//     vec3 n = cross(b, t);
//     F = mat3(b, n, t);

//     return p;
// }

vec3 bezier(float u, vec3 B[4], out mat3 F)
{
    vec3 beta[4];
    for (int i = 0; i < 4; i++)
        beta[i] = B[i];

    vec3 p, dp, d2p;
    for (int i = 1; i < 4; i++) {
        for (int j = 0; j < 4 - i; j++)
            beta[j] = (1 - u) * beta[j] + u * beta[j + 1];

        if (i == 1)
            d2p = cross(beta[1] - beta[2], beta[1] - beta[0]);
        if (i == 2)
            dp = beta[1] - beta[0];
        if (i == 3)
            p = beta[0];
    }

    vec3 t = normalize(dp);
    vec3 b = normalize(cross(dp, d2p));
    vec3 n = cross(b, t);
    F = mat3(b, n, t);

    return p;
}

vec3 cylBezierYZ(float u, float v, vec3 B[4], float r, out vec3 n)
{
    v *= 2 * M_PI;

    mat3 F;
    vec3 q = bezier(u, B, F);
    F[0] = vec3(0, 1, 0);
    F[1] = cross(F[2], F[0]);

    vec3 c = vec3(cos(v), sin(v), 0);
    float cn = abs(c.x) + abs(c.y);
    c /= cn;

    float theta = r_coef * u * 2 * M_PI;
    mat3 r_mat = mat3(vec3(cos(theta), -sin(theta), 0),
                      vec3(sin(theta), cos(theta), 0),
                      vec3(0, 0, 1));
    vec3 p = q + F * (r_mat * (r * c));

    n = normalize(normal_mat * (p - q));
    return p;
}

vec3 cylBezier(float u, float v, vec3 B[4], float r, out vec3 n)
{
    v *= 2 * M_PI;

    mat3 F;
    vec3 q = bezier(u, B, F);

    vec3 c = vec3(cos(v), sin(v), 0);
    float cn = abs(c.x) + abs(c.y);
    c /= cn;

    float theta = r_coef * u * 2 * M_PI;
    mat3 r_mat = mat3(vec3(cos(theta), -sin(theta), 0),
                      vec3(sin(theta), cos(theta), 0),
                      vec3(0, 0, 1));
    vec3 p = q + F * (r_mat * (r * c));

    n = normalize(normal_mat * (p - q));
    return p;
}   

void main()
{
    v_uv = vtx_texcoord;
    v_normal = normalize(normal_mat * vtx_normal);

    // vec3 p = cylinder(vtx_texcoord, vec3(0, 0, 0), vec3(1, 0, 1), 1);

    vec3 B[4] = B_1;
    vec3 p = cylBezier(vtx_texcoord.x, vtx_texcoord.y, B, .2, v_normal);

    // gl_Position = proj_mat * (view_mat * (obj_mat * vec4(vtx_position, 1)));
    gl_Position = proj_mat * (view_mat * (obj_mat * vec4(p, 1)));
}
