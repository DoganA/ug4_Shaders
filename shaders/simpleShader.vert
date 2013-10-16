// Vertex Shader - acts at a per-vertex level
#version 120

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;
uniform vec3 ambientReflectance, ambientIntensity;
uniform vec3 diffuseReflectance, diffuseIntensity;
uniform vec3 specularReflectance, specularIntensity;
uniform float shininess, constantAttenuation, linearAttenuation;

attribute vec3 vertex_position, vertex_normal;
varying vec4 vertex_color;

void main(void) {
    vec3 N = normalize(gl_NormalMatrix * vertex_normal); // normal
    vec3 L = normalize(vec3(gl_LightSource[0].position)); // light
    vec3 R = gl_LightSource[0].halfVector.xyz; // reflection

    vec3 materialAmbient = ambientReflectance * ambientIntensity;
    vec3 materialDiffuse = diffuseIntensity * diffuseReflectance;
    vec3 materialSpecular = specularIntensity * specularReflectance;

    vec4 ambientGlobal = vec4(materialAmbient, 1.0) * gl_LightModel.ambient;
    vec4 ambient = vec4(materialAmbient, 1.0) * gl_LightSource[0].ambient;
    vec4 diffuse = vec4(materialDiffuse, 1.0) * gl_LightSource[0].diffuse;

    float cosTheta = max(dot(L, N), 0.0);
    float cosAlpha = max(dot(N, R), 0.0);

    float dist = length(L);
    float attenuation = 1.0 / (constantAttenuation + dist * linearAttenuation);

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
