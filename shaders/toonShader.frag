// Fragment Shader - acts at a per-pixel level
#version 120

varying vec3 pass_color;

void main(void) {
    vec4 color;
    float intensity = (pass_color.x + pass_color.y + pass_color.z) / 3.0;

    if (intensity > 0.95) {
        color = vec4(0.99, 0.65, 0.24, 1.0);
    } else if (intensity > 0.75) {
        color = vec4(0.98, 0.43, 0.04, 1.0);
    } else if (intensity > 0.25) {
        color = vec4(0.79, 0.25, 0.04, 1.0);
    } else {
        color = vec4(0.49, 0.21, 0.04, 1.0);
    }

    gl_FragColor = color;
}
