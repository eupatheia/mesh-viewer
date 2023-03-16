#version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vUV;

uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;
uniform bool HasUV;

out vec3 color;

void main() {
  // calculate color based on normal
  color = (vNormal + 1) / 2.0;
  gl_Position = MVP * vec4(vPos, 1.0);
}
