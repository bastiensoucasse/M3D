#version 410 core

in vec3 frag_view;
in vec3 frag_tangent;
in vec3 frag_bitangent;
in vec3 frag_normal;
in vec4 frag_color;
in vec2 frag_texcoords;

out vec4 out_color;

uniform vec3 light_direction;
uniform sampler2D sampler0;
uniform sampler2D sampler1;

vec4 blinn(vec3 n, vec3 v, vec3 l, vec4 dc, vec4 sc, float s)
{
    // Ambient
    float at = 0.2;
    vec4 ac = at * dc;

    // Diffuse
    float dt = max(dot(n, l), 0);
    dc = dt * dc;

    // Specular
    float st = pow(max(dot(n, normalize(v + l)), 0), s);
    sc = st * sc;

    return ac + dc + sc;
}

void main()
{
    vec4 diffuse_texture = texture(sampler0, frag_texcoords);
    vec4 normals_texture = texture(sampler1, frag_texcoords);

    vec3 t = normalize(frag_tangent);
    vec3 b = normalize(frag_bitangent);
    vec3 n = normalize(frag_normal);
    mat3 tbn = mat3(t, b, n);

    // Activity version
    // vec3 normal = normalize(normals_texture.rgb * 2 - 1);
    // vec3 view = normalize(tbn * frag_view);
    // vec3 light = normalize(tbn * light_direction);

    // Lesson version
    vec3 normal = normalize(tbn * (normals_texture.rgb * 2 - 1));
    vec3 view = normalize(frag_view);
    vec3 light = normalize(light_direction);

    vec4 sc = vec4(1, 1, 1, 1);
    float s = 50;
    out_color = blinn(normal, view, light, diffuse_texture, sc, s);
}
