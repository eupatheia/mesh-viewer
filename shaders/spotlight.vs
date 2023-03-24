/* spotlight.vs
 * vertex shader for spotlighting
 * @author JL
 * @version March 23, 2023
 */

#version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormals;
layout (location = 2) in vec2 vTextureCoords;

struct Light {
  vec4 pos;
  vec3 La;
  vec3 Ld;
  vec3 Ls;
  float exponent;  // speed of decrease in brightness within spotlight
  // angle between center axis and edge of spotlight cone
  // in degrees in range [0, 90]
  float cutoff;
};

uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;
uniform bool HasUV;
uniform Light light;

out vec3 Pos;
out vec3 Norm;
out vec3 SpotDir;  // spotlight direction

void main() {
  // convert vertex position and vertex normal to eye coordinates
  Pos = vec3(ModelViewMatrix * vec4(vPos, 1.0));
  Norm = normalize(NormalMatrix * vNormals);
  // normalized spotlight direction in eye coords
  SpotDir = normalize(-(light.pos.xyz));
  gl_Position = MVP * vec4(vPos, 1.0);
}
