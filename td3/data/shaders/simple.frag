#version 330 core

in vec3 var_color;
out vec4 out_color;
uniform int mode;

void main(void)
{
    if (mode == 1)
        out_color = vec4(1.);
    else
        out_color = vec4(var_color, 1.);
}
