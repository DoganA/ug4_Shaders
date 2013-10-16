// Vertex Shader - acts at a per-vertex level
#version 120

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;
uniform vec3 ambientReflectance, ambientIntensity;
uniform vec3 diffuseReflectance, diffuseIntensity;
uniform vec3 specularReflectance, specularIntensity;
uniform float shininess, constantAttenuation, linearAttenuation;

attribute vec3 vertex_position, vertex_normal;
varying vec3 vertex_color;

void main(void) {
    vec3 N = normalize(gl_NormalMatrix * vertex_normal); // normal
    vec3 L = normalize(vec3(gl_LightSource[0].position)); // light
    vec3 R = gl_LightSource[0].halfVector.xyz; // reflection

    vec3 ambient = ambientReflectance * ambientIntensity;

    float cosTheta = max(dot(L, N), 0.0);
    vec3 diffuse = diffuseIntensity * diffuseReflectance * cosTheta;

    float cosAlpha = cosTheta > 0.0 ? max(dot(N, R), 0.0) : 0.0f;
    vec3 specular = specularIntensity * specularReflectance * pow(cosAlpha, shininess);

    float dist = length(L);
    float attenuation = 1.0 / (constantAttenuation + dist * linearAttenuation);

    vertex_color = ambient + attenuation * (diffuse + specular);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex_position, 1.0);
}
