// Vertex Shader - acts at a per-vertex level
#version 120

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

attribute vec3 vertex_position;
attribute vec2 vertex_uv;

varying vec2 uv;

void main(void) {
    vec4 vertex = vec4(vertex_position, 1.0);

    uv = vertex_uv;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vertex;
}
