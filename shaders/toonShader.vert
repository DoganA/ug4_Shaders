// Vertex Shader - acts at a per-vertex level
#version 120

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 materialAmbient, materialDiffuse, materialSpecular;
uniform float shininess, constantAttenuation, linearAttenuation;

attribute vec3 vertex_position, vertex_normal;
varying vec4 vertex_color, position;
varying vec3 normal;

void main(void) {
    vec4 vertex = vec4(vertex_position, 1.0);

    normal = normalize(normalMatrix * vertex_normal);
    position = viewMatrix * modelMatrix * vertex;
    vertex_color = vec4(materialAmbient, 1.0);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vertex;
}
