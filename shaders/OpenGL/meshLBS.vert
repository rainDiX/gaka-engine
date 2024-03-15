#version 450 core

#define MAX_BONES 100
#define VERTEX_BONES 4

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in int in_bone_count;
layout(location = 4) in ivec4 in_bone_idx;
layout(location = 5) in vec4 in_bone_weights;

layout(location = 0) out vec3 position_world;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 bones[MAX_BONES];

void main() {
    vec4 final_position = vec4(0.0);
    vec4 final_normal = vec4(0.0);

    for(int i = 0; i < in_bone_count; ++i) {
        if(in_bone_weights[i] == 0.0)
            continue;
        mat4 bone = bones[in_bone_idx[i]];
        vec4 pos = bone * vec4(in_position, 1.0) * in_bone_weights[i];

        final_position += pos;
        final_normal += (bone * vec4(in_normal, 0.0)) * in_bone_weights[i];
    }
    final_normal = normalize(final_normal);

    vec4 pos_world = model * final_position;
    position_world = vec3(pos_world);
    normal = mat3(transpose(inverse(model))) * vec3(final_normal);
    uv = in_uv;
    gl_Position = projection * view * pos_world;
}
