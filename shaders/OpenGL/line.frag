#version 450 core
out vec4 color;

in vec3 position_world;
in vec3 position_view;
in vec3 normal;
in vec2 uv;


void main() {
   color = vec4(0.5 * uv.x, 0.5*uv.y , 0.5, 1.0);
}
