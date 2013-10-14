// Vertex Shader - acts at a per-vertex level
#version 130

// set floats to have best precision
#ifdef GL_FRAGMENT_PRECISION_HIGH
    precision highp float;
#else
    precision mediump float;
#endif


uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 in_position;
out vec3 pass_color;

void main(void) {
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_position, 1.0);

  pass_color = vec3(gl_Color);
}
