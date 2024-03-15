#version 450 core
out vec4 color;

layout(location = 0) in vec3 position_world;
layout(location = 1) in vec3 position_view;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

uniform vec4 baseColorFactor;
uniform float metallicFactor;
uniform float roughnessFactor;

uniform int nb_point_lights;
uniform PointLight pointLights[MAX_POINTS_LIGHTS];

uniform bool hasTex;
uniform sampler2D tex;
uniform sampler2D baseColorTexture;
uniform sampler2D metallicRoughnessTexture;

vec3 diffuseBRDF() {
    vec3 diffuse = vec3(0.0);
    for(int i = 0; i < nb_point_lights; i++) {
        PointLight light = pointLights[i];
        vec3 light_dir = normalize(light.position - position_world);
        vec3 reflect_dir = reflect(-light_dir, normal);
        float distance = length(light.position - position_world);
        float attenuation = 1.0;

        vec3 view_dir = normalize(position_view - position_world);

        if(light.range > 0.0 && distance > light.range) {
            attenuation = 0.0;
        } else if(light.decay > 0.0) {
            attenuation = 1.0 / (1.0 + light.decay * pow(distance / light.range, 2.0));
        }

        diffuse += attenuation * material.diffuse * light.color * light.intensity * max(dot(normal, light_dir), 0.0);
    }
    return diffuse;
}

vec4 conductor_fresnel(f0, bsdf) {
}

vec3 fresnel_conductor(float cos_theta_i, float eta) {
    float r_parallel = 0.0;
    float r_perpendicular = 0.0;
    //?
    return (1 / 2) * (pow(r_parallel, 2) + pow(r_perpendicular, 2));
}

vec4 metalBRDF() {
    alpha = pow(roughnessFactor, 2);
    bsdf = specular_brdf(alpha);
    return conductor_fresnel(baseColor, bsdf);
}

vec4 dielectricBRDF() {
}

vec4 mix(dielectric_brdf, metal_brdf, metallic) {
    return (1.0 - metallic) * dielectric_brdf + metallic * metal_brdf;
}

void main() {
    return mix(dielectric_brdf, metal_brdf, metallic);
}
