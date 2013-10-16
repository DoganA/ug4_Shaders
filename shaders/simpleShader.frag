// Fragment Shader - acts at a per-pixel level
#version 120

varying vec4 vertex_color;

void main(void) {
    gl_FragColor = vertex_color;
}
