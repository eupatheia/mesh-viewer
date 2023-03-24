/* toon.fs
 * fragment shader for toon shading
 * @author JL
 * @version March 23, 2023
 */

#version 400

struct Light {
  vec4 pos;
  vec3 La;
  vec3 Ld;
  vec3 Ls;
};

struct Material {
  float shine;
  vec3 Ka;
  vec3 Kd;
  vec3 Ks;
};

uniform Light light;
uniform Material material;

in vec3 Pos;
in vec3 Norm;
out vec4 FragColor;

const int levels = 3;  // distinct "stripes" of gradation
const float scaleFactor = 1.0 / levels;

vec3 toonShade() {
  // normalized direction to light source
  vec3 s = normalize(vec3(light.pos) - Pos);
  // get cosine values between 0 and 1
  float cosine = max(0.0, dot(s, Norm));
  // calculate diffuse component (convert cosine to {0, 1, ... , levels -1},
  // then divide by levels to get levels levels between 0 and 1)
  vec3 diffuse = material.Kd * floor(cosine * levels) * scaleFactor;
  // add ambient and diffuse components
  return light.La * (material.Ka + diffuse);
}

void main() {
  FragColor = vec4(toonShade(), 1.0);
}
