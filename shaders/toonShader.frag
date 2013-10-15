// Fragment Shader - acts at a per-pixel level
#version 120

varying vec3 pass_color;

void main(void) {
    vec4 color;
    float intensity = (pass_color.x + pass_color.y + pass_color.z) / 3.0;

    if (intensity > 0.95) {
        color = vec4(1.0, 0.5, 0.5, 1.0);
    } else if (intensity > 0.5) {
        color = vec4(0.6, 0.3, 0.3, 1.0);
    } else if (intensity > 0.25) {
        color = vec4(0.4, 0.2, 0.2, 1.0);
    } else {
        color = vec4(0.2, 0.1, 0.1, 1.0);
    }

    gl_FragColor = color;
}
