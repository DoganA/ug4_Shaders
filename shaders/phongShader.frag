// Fragment Shader - acts at a per-pixel level
#version 120

varying vec4 diffuse, ambientGlobal, ambient, position;
varying vec3 normal, reflection;

void main(void) {
    vec3 Is = vec3(0.95, 0.95, 0.95); // specular light intensity
    vec3 ks = vec3(0.99, 0.99, 0.99); // specular reflectance
    float n = 100; // shiny reflectance of object
    vec4 materialSpecular = vec4(Is * ks, 1.0);

    float constantAttenuation = 50;
    float linearAttenuation = 0.8;

    vec4 color = ambientGlobal;
    vec3 N = normalize(normal);
    vec3 R = normalize(reflection);
    vec3 L = vec3(gl_LightSource[0].position - position);
    float dist = length(L);
    float cosTheta = max(dot(L, N), 0.0);
    float cosAlpha = max(dot(N, R), 0.0);
    float attenuation = 1.0 / (constantAttenuation + dist * linearAttenuation);

    if (cosTheta > 0.0) {
        color += attenuation * (diffuse * cosTheta + ambient);
        color +=   attenuation
                 * materialSpecular
                 * gl_LightSource[0].specular
                 * pow(cosAlpha, n);
    }

    gl_FragColor = color;
}
