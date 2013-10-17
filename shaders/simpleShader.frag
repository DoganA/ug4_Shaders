// Fragment Shader - acts at a per-pixel level
#version 120

uniform sampler2D texture0;
uniform int useTexture;

varying vec3 vertex_color;
varying vec2 uv;

void main(void) {
    vec3 color = vertex_color;

    if (useTexture != 0) color *= texture2D(texture0, uv.st).rgb;
    gl_FragColor = vec4(color, 1.0);
}
