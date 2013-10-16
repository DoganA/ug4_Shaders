// Fragment Shader - acts at a per-pixel level
#version 120

uniform vec3 materialSpecular;
uniform float shininess, constantAttenuation, linearAttenuation;

varying vec4 diffuse, ambientGlobal, ambient, position;
varying vec3 normal, reflection;

void main(void) {
    vec3 N = normalize(normal);
    vec3 R = normalize(reflection);
    vec3 L = vec3(gl_LightSource[0].position - position);

    float cosTheta = max(dot(L, N), 0.0);
    float cosAlpha = max(dot(N, R), 0.0);

    float attenuation = 1.0 / (constantAttenuation + length(L) * linearAttenuation);

    vec4 color = ambientGlobal;
    if (cosTheta > 0.0) {
        color += attenuation * (diffuse * cosTheta + ambient);
        color +=   attenuation
                 * vec4(materialSpecular, 1.0)
                 * gl_LightSource[0].specular
                 * pow(cosAlpha, shininess);
    }

    gl_FragColor = color;
}
