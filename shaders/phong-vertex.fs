/* phong-vertex.fs
 * fragment shader for per-vertex phong shading
 * @author JL
 * @version March 16, 2023
 */

#version 400

in vec3 lightIntensity;
out vec4 FragColor;

void main() {
  FragColor = vec4(lightIntensity, 1.0);
}
