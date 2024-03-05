#version 450 core

layout(location=0) in vec3 in_position;

layout(location=0) out vec3 position;

void main() {
    vec4 pos_world = model * vec4(in_position, 1.0);
    vec4 pos_view = view * pos_world;

    position_world = vec3(pos_world);
    normal = normalize(mat3(model) * in_normal);
    position_view = vec3(pos_view);
    uv = in_uv;

    gl_Position = projection * pos_view;
}
