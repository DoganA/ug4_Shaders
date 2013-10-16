// Fragment Shader - acts at a per-pixel level
#version 120

varying float depth;

void main(void) {
    float d = 1.0 - depth;
    gl_FragColor = vec4(d, d, d, 1.0);
}
