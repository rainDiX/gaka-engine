#version 450 core
out vec4 color;

layout(location = 0) in vec3 position_world;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform bool hasTex;
uniform sampler2D tex;

void main() {
   color = vec4(0.5 * normal.x + 0.5, 0.5 * normal.y + 0.5, 0.5 * normal.z + 0.5, 1.0);
   if(hasTex) {
      color *= texture(tex, uv);
   }
}
