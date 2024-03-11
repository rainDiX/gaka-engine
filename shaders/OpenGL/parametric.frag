#version 450 core
out vec4 color;

layout(location=0) in vec3 position_world;
layout(location=1) in vec3 position_view;
layout(location=2) in vec3 normal;
layout(location=3) in vec2 uv;

void main() {
   color = vec4(0.5 * uv.x + 0.5, 0.5 * uv.y + 0.5, 1.0, 1.0);
}
