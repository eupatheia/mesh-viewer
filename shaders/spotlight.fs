/* spotlight.fs
 * fragment shader for spotlighting
 * @author JL
 * @version March 23, 2023
 */

#version 400

struct Light {
  vec4 pos;  // spotlight location in eye coordinates
  vec3 La;
  vec3 Ld;
  vec3 Ls;
  float exponent;  // speed of decrease in brightness within spotlight
  // angle between center axis and edge of spotlight cone
  // in degrees in range [0, 90]
  float cutoff;
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
in vec3 SpotDir;  // direction of spotlight (center axis) in eye coords
out vec4 FragColor;

vec3 spotlightShade() {
  // normalized direction to light source
  vec3 s = normalize(vec3(light.pos) - Pos);
  // angle between center of spotlight and light-to-pos vector
  float angle = acos(dot(-s, SpotDir));
  // clamp cutoff angle to 90 deg and convert to rads
  float cutoff = radians(clamp(light.cutoff, 0.0, 90.0));
  // compute ambient light
  vec3 ambient = light.La * material.Ka;
  if (angle < cutoff) {
    // within spotlight, calculate brightness based on distance to core
    float spotFactor = pow(dot(-s, SpotDir), light.exponent);
    vec3 v = normalize(vec3(-Pos));
    vec3 h = normalize(v + s);
    return ambient + (spotFactor * light.La *
                (material.Kd * max(dot(s, Norm), 0.0)) + 
                (material.Ks * pow(max(dot(h, Norm), 0.0), material.shine)));
  } else {
    return ambient;
  }
}

void main() {
  FragColor = vec4(spotlightShade(), 1.0);
}
