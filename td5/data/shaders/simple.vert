#version 410 core

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_color;

// out vec3 var_view;
// out vec3 var_normal;
out vec3 var_color;

uniform mat4 transform_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat3 normal_matrix;
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

    return ambient_color + diffuse_color + specular_color;
}

void main()
{
    vec4 local_position = vec4(vertex_position, 1);
    vec4 world_position = transform_matrix * local_position;
    vec4 camera_position = view_matrix * world_position;

    vec3 var_view = normalize(-camera_position.xyz);
    vec3 var_normal = normalize(normal_matrix * vertex_normal);
    var_color = blinn(var_normal, var_view, light_direction, vec3(0, 0, var_normal.z), light_color, 50);

    gl_Position = projection_matrix * camera_position;
}
