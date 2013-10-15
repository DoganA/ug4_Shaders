// Vertex Shader - acts at a per-vertex level
#version 120

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

attribute vec3 in_position, in_normal;
varying vec3 pass_color;

void main(void) {
    vec4 vertex = vec4(in_position, 1.0);
    vec3 normal = in_normal;

    vec3 lightDir = normalize(vec3(gl_LightSource[0].position));
    float intensity = dot(lightDir, normal);

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vertex;
    pass_color = vec3(intensity);
}
