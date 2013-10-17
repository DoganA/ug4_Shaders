// Vertex Shader - acts at a per-vertex level
#version 120

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 materialAmbient, materialDiffuse, materialSpecular;
uniform float shininess, constantAttenuation, linearAttenuation;

attribute vec3 vertex_position, vertex_normal;
varying vec3 vertex_color, position, normal;

void main(void) {
    vec4 vertex = vec4(vertex_position, 1.0);

    normal = normalize(normalMatrix * vertex_normal);
    position = vec3(viewMatrix * modelMatrix * vertex);
    vertex_color = materialAmbient + materialDiffuse;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vertex;
}
