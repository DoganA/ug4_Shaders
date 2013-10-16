// Vertex Shader - acts at a per-vertex level
#version 120

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

attribute vec3 vertex_position;
varying float depth;

void main(void) {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex_position, 1.0);
    depth = gl_Position.z / gl_Position.w;
}
