#version 330 core

in vec3 vtx_position;
in vec3 vtx_color;
out vec3 var_color;

uniform float zoom;
uniform vec2 translate;
uniform int mode;

void main()
{
    gl_Position = vec4(zoom * vtx_position.xy + translate, -vtx_position.z + (mode == 1 ? -1e-3 : 0), 1.);
    var_color = vtx_color;
}
