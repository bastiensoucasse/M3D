#version 410 core

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_color;

out vec3 var_view;
out vec3 var_normal;
out vec3 var_color;

uniform mat4 transform_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat3 normal_matrix;

// uniform vec3 light_direction;
// uniform vec3 light_color;

// vec3 blinn(vec3 normal, vec3 view_direction, vec3 light_direction, vec3 diffuse_color, vec3 specular_color, float shininess)
// {
//     vec3 blinn = .2 * diffuse_color;

//     float diffuse_term = max(dot(normal, light_direction), 0);
//     if (diffuse_term > 0)
//         blinn += diffuse_color * diffuse_term;

//     vec3 halfway_direction = normalize(view_direction + light_direction);
//     float specular_term = pow(max(dot(normal, halfway_direction), 0), shininess);    
//     if (diffuse_term > 0 && specular_term > 0)
//         blinn += specular_color * specular_term * diffuse_term;

//     return blinn;
// }

void main()
{
    vec4 local_position = vec4(vertex_position, 1);
    vec4 world_position = transform_matrix * local_position;
    vec4 camera_position = view_matrix * world_position;

    var_view = normalize(-camera_position.xyz);
    var_normal = normalize(normal_matrix * vertex_normal);
    var_color = vertex_color;

    // float shininess = 50;
    // var_color = blinn(var_normal, var_view, light_direction, vertex_color, light_color, shininess);
    // var_color = blinn(var_normal, var_view, light_direction, vec3(0, 0, var_normal.z), light_color, shininess);

    gl_Position = projection_matrix * camera_position;
}
