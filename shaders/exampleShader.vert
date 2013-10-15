// Vertex Shader - acts at a per-vertex level
#version 120

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

attribute vec3 in_position;
varying vec3 pass_color;

void main(void) {
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_position, 1.0);

  pass_color = vec3(gl_Color);
}
