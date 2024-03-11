#version 450 core

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec2 in_uv;

layout(location=0) out vec3 position_world;
layout(location=1) out vec3 position_view;
layout(location=2) out vec3 normal;
layout(location=3) out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 pos_world = model * vec4(in_position, 1.0);
    vec4 pos_view = view * pos_world;

    position_world = vec3(pos_world);
    normal = normalize(mat3(model) * in_normal);
    position_view = vec3(pos_view);
    uv = in_uv;

    gl_Position = projection * pos_view;
}
