#version 450 core
out vec4 color;

layout(location=0) in vec3 position_world;
layout(location=1) in vec3 position_view;
layout(location=2) in vec3 normal;
layout(location=3) in vec2 uv;

void main() {
   float e = (uv.x + uv.y) * 0.00000000000001;
   color = vec4(0.5 * normal.x + 0.5, 0.5 * normal.y + 0.5, 0.5 * normal.z + 0.5 + e, 1.0);
}
