// Fragment Shader - acts at a per-pixel level
#version 120

varying vec3 pass_color;

void main(void) {
  gl_FragColor = vec4(pass_color,1.0);
}
