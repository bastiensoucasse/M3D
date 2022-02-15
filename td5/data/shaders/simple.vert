#version 410 core

in vec3 vtx_position;
in vec3 vtx_color;
out vec3 var_color;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

vec3 blinn(vec3 n, vec3 v, vec3 l, vec3 dCol, vec3 sCol, float s)
{
    vec3 r = -l + 2. * dot(l, n) * n;
    float a = dot(v, r);

    vec3 col = dCol;

    if (a > 0)
        col +=  sCol * pow(a, s);

    return col;
}

void main()
{
    vec4 local_position = vec4(vtx_position, 1.);
    vec4 world_position = transform * local_position;
    vec4 camera_position = view * world_position;
    gl_Position = projection * camera_position;

    vec3 v = camera_position.xyz;

    vec3 n = …;
    vec3 l = …;
    vec3 dCol = vtx_color;
    vec3 sCol = …;
    float s = …;

    var_color = blinn(n, v, l, dCol, sCol, s)
}
