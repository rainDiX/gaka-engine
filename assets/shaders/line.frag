#version 460 core
out vec4 color;

in vec3 position_world;
in vec3 position_view;
in vec3 normal;
in vec2 tex_coords;


void main() {
   color = vec4(0.5 * tex_coords.x, 0.5*tex_coords.y , 0.5, 1.0);
}