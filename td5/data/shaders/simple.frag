#version 410 core

in vec3 var_normal;
in vec3 var_view;
in vec3 var_color;

out vec4 out_color;

uniform vec3 light_direction;
uniform vec3 light_color;

vec3 blinn(vec3 normal, vec3 view_direction, vec3 light_direction, vec3 fragment_color, vec3 light_color, float shininess)
{
    float ambient_term = 0.2;
    vec3 ambient_color = fragment_color * ambient_term;

    float diffuse_term = max(dot(normal, light_direction), 0);
    vec3 diffuse_color = fragment_color * diffuse_term;

    float specular_term = pow(max(dot(normal, normalize(view_direction + light_direction)), 0), shininess);
    vec3 specular_color = light_color * specular_term;

    // return ambient_color;
    // return diffuse_color;
    // return specular_color;
    return ambient_color + diffuse_color + specular_color;
}

void main()
{
    float shininess = 50;

    // out_color = vec4(var_color, 1);
    out_color = vec4(blinn(normalize(var_normal), normalize(var_view), normalize(light_direction), var_color, light_color, shininess), 1);
}
