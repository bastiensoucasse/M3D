#version 410 core

in vec3 var_normal;
in vec3 var_view;
in vec3 var_color;
in vec2 var_texcoords;

out vec4 out_color;

uniform vec3 light_direction;
uniform vec3 light_color;
uniform sampler2D sampler0;
uniform sampler2D sampler1;
uniform sampler2D sampler2;

vec3 blinn(vec3 normal, vec3 view_direction, vec3 light_direction, vec3 fragment_color, vec3 light_color, float shininess)
{
    float ambient_term = 0.2;
    vec3 ambient_color = fragment_color * ambient_term;

    float diffuse_term = max(dot(normal, light_direction), 0);
    vec3 diffuse_color = fragment_color * diffuse_term;

    float specular_term = pow(max(dot(normal, normalize(view_direction + light_direction)), 0), shininess);
    vec3 specular_color = light_color * specular_term;

    return ambient_color + diffuse_color /* + specular_color */;
}

void main()
{
    float shininess = 50;
    
    // out_color = vec4(blinn(normalize(var_normal), normalize(var_view), normalize(light_direction), var_color, light_color, shininess), 1);

    vec4 tex0 = texture(sampler0, var_texcoords);
    vec4 tex1 = texture(sampler1, var_texcoords);
    vec4 tex2 = texture(sampler2, var_texcoords);
    vec4 mix1 = mix(tex2, mix(tex0, tex1, tex1.x), max(dot(normalize(var_normal), normalize(light_direction)), 0));
    out_color = vec4(blinn(normalize(var_normal), normalize(var_view), normalize(light_direction), mix1.xyz, light_color, shininess), 1);
}
