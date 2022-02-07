#version 330 core

in vec3 var_color;
out vec4 out_color;

uniform int wireframe;

void main(void) {
    if(wireframe==1)
        out_color = vec4(1,1,1,1);
    else
        out_color = vec4(var_color,1);
}
