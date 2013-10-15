// Fragment Shader - acts at a per-pixel level
#version 120

varying vec3 normal;

void main(void) {
    vec3 n = normalize(normal);
    float intensity = dot(vec3(gl_LightSource[0].position), n);

    vec4 color;
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
