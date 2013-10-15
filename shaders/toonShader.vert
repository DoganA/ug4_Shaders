// Vertex Shader - acts at a per-vertex level
#version 120

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

attribute vec3 in_position, in_normal;
varying vec3 normal;

void main(void) {
    normal = gl_NormalMatrix * in_normal;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_position, 1.0);
}
