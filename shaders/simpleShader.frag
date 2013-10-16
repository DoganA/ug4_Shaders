// Fragment Shader - acts at a per-pixel level
#version 120

varying vec3 vertex_color;

void main(void) {
    gl_FragColor = vec4(vertex_color, 1.0);
}
