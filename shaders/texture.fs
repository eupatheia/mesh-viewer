/* texture.fs
 * fragment shader for texture mapping
 * @author JL
 * @version March 22, 2023
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
uniform sampler2D diffuseTexture;

in vec3 Pos;
in vec3 Norm;
in vec2 TexCoord;
out vec4 FragColor;

void phongModel(in vec3 pos, in vec3 norm, out vec3 ambAndDiff,
    out vec3 spec) {
  // normalize normal
  vec3 n = normalize(norm);
  // normalized direction to light source
  vec3 s = normalize(vec3(light.pos) - pos);
  // normalized direction towards viewer
  vec3 v = normalize(vec3(-pos));
  // reflect -s about n
  vec3 r = reflect(-s, n);
  // add ambient, diffuse, and specular intensities
  ambAndDiff =  vec3(0.7, 0.7, 0.7) * (vec3(0.7, 0.7, 0.7) +
      (vec3(0.7, 0.7, 0.7) * max(dot(s, n), 0.0)));
  spec = vec3(0.7, 0.7, 0.7) * vec3(0.7, 0.7, 0.7) *
      pow(max(dot(r, v), 0.0), material.shine);
}

void main() {
  vec3 ambAndDiff, spec;
  // get texture color
  vec4 texColor = texture(diffuseTexture, TexCoord * 5.0f);
  phongModel(Pos, Norm, ambAndDiff, spec);
  FragColor = (vec4(ambAndDiff, 1.0) * texColor) + vec4(spec, 1.0);
}
