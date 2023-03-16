/* normals.fs
 * fragment shader for color by normal
 * @author JL
 * @version March 16, 2023
 */

#version 400

in vec3 color;
out vec4 FragColor;

void main() {
  FragColor = vec4(color, 1.0);
}