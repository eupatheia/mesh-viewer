/* lattice.fs
 * fragment shader for per-vertex phong shading with cutouts
 * @author JL
 * @version March 22, 2023
 */

#version 400

in vec3 FrontColor;
in vec3 BackColor;
in vec2 TexCoord;
out vec4 FragColor;

void main() {
  // number of lattice rectangles per scaled unit of texture coordinate
  const float scale = 15.0;
  // compute fractional component of scaled TexCoord and compare to cutoff
  bvec2 toDiscard = greaterThan(fract(TexCoord * scale), vec2(0.2, 0.2));
  // if within the cutout, stop execution w/o writing to output buffer
  if(all(toDiscard)) {
    discard;
  }
  // otherwise, color based on which side is being viewed
  if (gl_FrontFacing) {
    FragColor = vec4(FrontColor, 1.0);
  } else {
    FragColor = vec4(BackColor, 1.0);
  }
}
