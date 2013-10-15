// Vertex Shader - acts at a per-vertex level
#version 120

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

attribute vec3 vertex_position, vertex_normal;
varying vec3 color;

void main(void) {
    vec3 diffuseColor = vec3(1.0, 0.5, 0.0);
    float intensity = dot(vec3(gl_LightSource[0].position), vertex_normal);

    color = vec3(intensity) * diffuseColor;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex_position, 1.0);
}
