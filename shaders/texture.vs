/* texture.vs
 * vertex shader for texture mapping
 * @author JL
 * @version March 22, 2023
 */

#version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormals;
layout (location = 2) in vec2 vTextureCoords;

uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;
uniform bool HasUV;

out vec3 Pos;
out vec3 Norm;
out vec2 TexCoord;

void main() {
  // vertex position to eye space
  Pos = vec3(ModelViewMatrix * vec4(vPos, 1.0));
  // vertex normals to eye space, normalized
  Norm = normalize(NormalMatrix * vNormals);
  // pass texture coords without change
  TexCoord = vTextureCoords;
  gl_Position = MVP * vec4(vPos, 1.0);
}
