/* phong-pixel.fs
 * fragment shader for per-pixel phong shading
 * @author JL
 * @version March 16, 2023
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

in vec3 pos;
in vec3 norm;
out vec4 FragColor;

vec3 shade() {
  // normalize normal
  vec3 n = normalize(norm);
  // normalized direction to light source
  vec3 s = normalize(vec3(light.pos) - pos);
  // normalized direction towards viewer
  vec3 v = normalize(vec3(-pos));
  // reflect -s about n
  vec3 r = reflect(-s, n);
  // add ambient, diffuse, and specular intensities
  return light.Ls * (material.Ka +
                    (material.Kd * max(dot(s, n), 0.0)) +
                    (material.Ks * pow(max(dot(r, v), 0.0), material.shine)));
}

void main() {
  FragColor = vec4(shade(), 1.0);
}
