#version 450 core
layout(location = 0) out vec4 color;

// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#appendix-b-brdf-implementation

#define PI  3.1415927410125732421875
#define MAX_POINTS_LIGHTS 10

layout(location=0) in vec3 position_world;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

uniform vec4 baseColorFactor;
uniform float metallicFactor;
uniform float roughnessFactor;
uniform vec3 view_pos;

struct PointLight {
    vec3 color;
    float intensity;
    float range;
    float decay;
    vec3 position;
};

uniform int nb_point_lights;
uniform PointLight pointLights[MAX_POINTS_LIGHTS];
uniform bool hasTex;
uniform sampler2D tex;
uniform sampler2D baseColorTexture;
uniform sampler2D metallicRoughnessTexture;

vec3 mix_brdf(vec3 dielectric_brdf, vec3 metal_brdf, float metallic) {
    return (1.0 - metallic) * dielectric_brdf + metallic * metal_brdf;
}

vec3 diffuseBRDF(vec3 diffuseColor) {
    return diffuseColor / PI;
}

float Heaviside(float x) {
    return float(x > 0);
}

float visibilityFunction(vec3 V, vec3 N, vec3 L, vec3 H, float alpha2) {
    float NdotL = dot(N, L);
    float left = Heaviside(dot(H, L)) / (abs(NdotL) + sqrt(alpha2 + (1 - alpha2) * NdotL * NdotL));
    float NdotV = dot(N, V);
    float right = Heaviside(dot(H, V)) / (abs(NdotV) + sqrt(alpha2 + (1 - alpha2) * NdotV * NdotV));
    return left * right;
}

// Trowbridge-Reitz/GGX microfacet distribution
float GGX(vec3 N, vec3 H, float alpha2) {
    float NdotH = dot(N, H);
    return (alpha2 * Heaviside(NdotH)) / (PI * pow(NdotH * NdotH * (alpha2 - 1) + 1, 2));
}


vec3 specularBRDF(vec3 V, vec3 N, vec3 L, vec3 H, float alpha, vec3 f0) {
    float alpha2 = alpha * alpha;
    float VdotH = dot(V, H);
    float Vi = visibilityFunction(V, N, L, H, alpha2);
    float D = GGX(N, H, alpha2);
    vec3 F = f0 + (vec3(1.0) - f0) * pow((1 - abs(VdotH)), 5);
    return F * Vi * D;
}

vec3 fresnel_mix(float ior, vec3 base, vec3 layer, float VdotH) {
    float f0 = pow((1 - ior) / (1 + ior), 2);
    float fr = f0 + (1 - f0) * pow((1 - abs(VdotH)), 5);
    return mix_brdf(base, layer, fr);
}

void main() {
    vec4 baseColor = baseColorFactor;
    float metallic = metallicFactor;
    float roughness = roughnessFactor;

    if(hasTex) {
        baseColor = texture(baseColorTexture, uv);
        metallic = texture(metallicRoughnessTexture, uv).b;
        roughness = texture(metallicRoughnessTexture, uv).g;
    }

    vec3 material = vec3(0.0);

    //the surface normal in the same space as the above values
    vec3 N = normalize(normal);

    // normalized vector from the shading location to the eye
    vec3 V = normalize(view_pos - position_world);

    for(int i = 0; i < nb_point_lights; ++i) {
        // L is the normalized vector from the shading location to the light
        vec3 L = normalize(pointLights[i].position - position_world);
        // H is the half vector, where
        vec3 H = normalize(L + V);
        float VdotH = dot(V, H);

        vec3 specular_brdf = specularBRDF(V, N, L, H, roughness * roughness, baseColor.rgb);

        vec3 metal_brdf = specular_brdf * (baseColor.rgb + (vec3(1.0) - baseColor.rgb) * pow((1 - abs(VdotH)), 5));

        vec3 dielectric_brdf = fresnel_mix(1.5, diffuseBRDF(baseColor.rgb), specular_brdf, 0.04 + (1 - 0.04) * pow((1 - abs(VdotH)), 5));

        material += mix_brdf(dielectric_brdf, metal_brdf, metallic);

    }

    color = vec4(vec3(material.x, material.y , material.z), baseColor.a);
}
