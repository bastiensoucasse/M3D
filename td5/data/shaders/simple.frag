#version 410 core

in vec3 var_normal;
in vec3 var_view;
in vec3 var_color;

out vec4 out_color;

uniform vec3 light_direction;
uniform vec3 light_color;

vec3 blinn(vec3 normal, vec3 view_direction, vec3 light_direction, vec3 diffuse_color, vec3 specular_color, float shininess)
{
    vec3 blinn = .2 * diffuse_color;

    float diffuse_term = max(dot(normal, light_direction), 0);
    if (diffuse_term > 0)
        blinn += diffuse_color * diffuse_term;

    vec3 halfway_direction = normalize(view_direction + light_direction);
    float specular_term = pow(max(dot(normal, halfway_direction), 0), shininess);    
    if (diffuse_term > 0 && specular_term > 0)
        blinn += specular_color * specular_term * diffuse_term;

    return blinn;
}

void main()
{
    // out_color = vec4(var_color, 1);

    float shininess = 50.0;
    out_color = vec4(blinn(var_normal, var_view, light_direction, var_color, light_color, shininess), 1);
    // out_color = vec4(blinn(var_normal, var_view, light_direction, vec3(0, 0, var_normal.z), light_color, shininess), 1);
}
