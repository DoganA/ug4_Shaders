// Vertex Shader - acts at a per-vertex level
#version 120

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

attribute vec3 in_position, in_normal;
varying vec3 color;

void main(void) {
    vec3 diffuseColor = vec3(1.0, 0.5, 0.0);
    float intensity = dot(vec3(gl_LightSource[0].position), in_normal);

    color = vec3(intensity) * diffuseColor;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_position, 1.0);
}
