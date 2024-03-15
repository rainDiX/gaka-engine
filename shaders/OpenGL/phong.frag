#version 450 core
out vec4 color;

layout(location = 0) in vec3 position_world;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight {
    vec3 color;
    float intensity;
    float range;
    float decay;
    vec3 position;
};

#define MAX_POINTS_LIGHTS 10

uniform int nb_point_lights;
uniform mat4 model;
uniform vec3 view_pos;
uniform PointLight pointLights[MAX_POINTS_LIGHTS];
uniform Material material;

uniform bool hasTex;
uniform sampler2D tex;

vec3 calculatePointLight(PointLight light) {
    vec3 light_dir = normalize(light.position - position_world);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float distance = length(light.position - position_world);

    float attenuation = 1.0;

    vec3 view_dir = normalize(view_pos - position_world);

    if(light.range > 0.0 && distance > light.range) {
        attenuation = 0.0;
    } else if(light.decay > 0.0) {
        attenuation = 1.0 / (1.0 + light.decay * pow(distance / light.range, 2.0));
    }

    float diff = max(dot(normal, light_dir), 0.0);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    vec3 ambient = material.ambient * light.color * light.intensity;
    vec3 diffuse = material.diffuse * light.color * light.intensity * diff;
    vec3 specular = material.specular * light.color * light.intensity * spec;

    return (ambient + diffuse + specular) * attenuation;
}

void main() {
    vec3 result = vec3(0.0);

    for(int i = 0; i < nb_point_lights; i++) result += calculatePointLight(pointLights[i]);

    color = vec4(result, 1.0);
    if(hasTex) {
        color *= texture(tex, uv);
    }
}
