// Fragment Shader - acts at a per-pixel level
#version 120

varying vec3 color;

void main(void) {
    gl_FragColor = vec4(color, 1.0);
}
