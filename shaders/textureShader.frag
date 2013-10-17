// Fragment Shader - acts at a per-pixel level
#version 120

uniform sampler2D texture0;

varying vec3 vertex_color;
varying vec2 uv;

void main(void) {
    gl_FragColor = vec4(vertex_color, 1.0) * texture2D(texture0, uv.st);
}

