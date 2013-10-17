// Fragment Shader - acts at a per-pixel level
#version 120

uniform vec3 lightPos;
uniform sampler2D texture0;
uniform sampler2D bumpmap1;

varying vec2 uv;

void main(void) {
    vec3 N = normalize((texture2D(bumpmap1, uv.st)).rgb * 2.0 - 1.0);
    vec3 L = normalize(lightPos);

    float diffuse = max(dot(N, L), 0.0);
    vec3 color = diffuse * texture2D(texture0, uv.st).rgb;

    gl_FragColor = vec4(color, 1.0);
}
