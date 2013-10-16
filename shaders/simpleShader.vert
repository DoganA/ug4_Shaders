// Vertex Shader - acts at a per-vertex level
#version 120

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 materialAmbient, materialDiffuse, materialSpecular;
uniform float shininess, constantAttenuation, linearAttenuation;

attribute vec3 vertex_position, vertex_normal;
varying vec4 vertex_color;

void main(void) {
    vec4 ambientGlobal = vec4(materialAmbient, 1.0) * gl_LightModel.ambient;
    vec4 ambient = vec4(materialAmbient, 1.0) * gl_LightSource[0].ambient;
    vec4 diffuse = vec4(materialDiffuse, 1.0) * gl_LightSource[0].diffuse;

    vec3 N = normalize(normalMatrix * vertex_normal); // normal
    vec3 R = gl_LightSource[0].halfVector.xyz; // reflection
    vec3 L = normalize(vec3(gl_LightSource[0].position)); // light

    float cosTheta = max(dot(L, N), 0.0);
    float cosAlpha = max(dot(N, R), 0.0);

    float attenuation = 1.0 / (constantAttenuation + length(L) * linearAttenuation);

    vec4 color = ambientGlobal;
    if (cosTheta > 0.0) {
        color += attenuation * (diffuse * cosTheta + ambient);
        color +=  attenuation
                * vec4(materialSpecular, 1.0)
                * gl_LightSource[0].specular
                * pow(cosAlpha, shininess);
    }

    vertex_color = color;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex_position, 1.0);
}
