#version 410 core

in vec3 vtx_position;
in vec3 vtx_color;
out vec3 var_color;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
uniform int mode;

void main()
{
    gl_Position = projection * (view * transform * vec4(vtx_position, 1.));

    if (mode == 1)
        var_color = vec3(1., 1., 1.);
    else
        var_color = vtx_color;
}
