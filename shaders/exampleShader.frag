// Fragment Shader - acts at a per-pixel level
#version 130

// set floats to have best precision
#ifdef GL_FRAGMENT_PRECISION_HIGH
    precision highp float;
#else
    precision mediump float;
#endif

in vec3 pass_color;

void main(void) {
  gl_FragColor = vec4(pass_color,1.0);
}
