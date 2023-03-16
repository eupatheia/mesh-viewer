#version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormals;
layout (location = 2) in vec2 vTextureCoords;

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

uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;
uniform bool HasUV;
uniform Light light;
uniform Material material;

out vec3 lightIntensity;

void main() {
  // vertex normal to eye coordinates, then normalize
  vec3 tnorm = normalize(NormalMatrix * vNormals);
  // vertex position to eye coordinates
  vec4 eyeCoords = ModelViewMatrix * vec4(vPos, 1.0);
  // normalized direction to light source
  vec3 s = normalize(vec3(light.pos - eyeCoords));
  // normalized direction towards viewer
  vec3 v = normalize(-eyeCoords.xyz);
  // pure reflection of -s about tnorm
  vec3 r = reflect(-s, tnorm);
  // clamping prevents obtuse angles, i.e. light from inside surface
  float sDotN = max(dot(s, tnorm), 0.0);

  vec3 ambient = light.La * material.Ka;
  vec3 diffuse = light.Ld * material.Kd * sDotN;
  vec3 specular = vec3(0.0);
  if (sDotN > 0.0) {
    specular = light.Ls * material.Ks * pow(max(dot(r, v), 0.0),
        material.shine);
  }
  lightIntensity = ambient + diffuse + specular;
  gl_Position = MVP * vec4(vPos, 1.0);
}
