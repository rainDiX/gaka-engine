#version 450 core

in vec3 in_position;
in vec3 in_normal;
in vec2 in_tex_coords;

out vec3 position_world;
out vec3 position_view;
out vec3 position;
out vec3 normal;
out vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 pos_world = model * vec4(in_position, 1.0);
    vec4 pos_view = view * pos_world;

    position_world = vec3(pos_world);
    normal = normalize(mat3(model) * in_normal);
    position_view = vec3(pos_view);
    tex_coords = in_tex_coords;

    gl_Position = projection * pos_view;
}
