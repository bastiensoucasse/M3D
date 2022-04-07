#version 410 core

#define M_PI 3.14159265358979323846264338327950288
#define EPSILON 0.2

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
    vec4 cylinder = vec4(r * cos(uv.x * 2 * M_PI), r * sin(uv.x * 2 * M_PI), z_pos, 1);

    float z_p1 = L.x;
    float z_p2 = L.x + L.y;
    float z_p3 = L.x + L.y + L.z;

    int segment = -1, segment2 = -1;
    float weight = -1;
    if (z_pos >= 0 && z_pos < z_p1) {
        segment = 0;

        float d_p1 = distance(z_pos, z_p1);
        if (d_p1 < EPSILON) {
            segment2 = 1;
            weight = d_p1;
        }
    }
    if (z_pos >= z_p1 && z_pos < z_p2) {
        segment = 1;

        float d_p1 = distance(z_pos, z_p1);
        if (d_p1 < EPSILON) {
            segment2 = 0;
            weight = d_p1;
        }

        float d_p2 = distance(z_pos, z_p2);
        if (d_p2 < EPSILON) {
            segment2 = 2;
            weight = d_p2;
        }
    }
    if (z_pos >= z_p2 && z_pos <= z_p3) {
        segment = 2;

        float d_p2 = distance(z_pos, z_p2);
        if (d_p2 < EPSILON) {
            segment2 = 1;
            weight = d_p2;
        }
    }

    // Fatal error
    if (segment == -1)
        return vec4(0, 0, 0, 0);

    // No interpolation.
    if (weight == -1 || segment2 == -1)
        return M[segment] * cylinder;

    return weight * M[segment] * cylinder + (1 - weight) * M[segment2] * cylinder;
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
