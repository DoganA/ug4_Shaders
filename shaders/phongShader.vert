// Vertex Shader - acts at a per-vertex level
#version 120

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 materialAmbient, materialDiffuse;

attribute vec3 vertex_position, vertex_normal;
varying vec4 diffuse, ambientGlobal, ambient, position;
varying vec3 normal, reflection;

void main(void) {
    vec4 vertex = vec4(vertex_position, 1.0);

    normal = normalize(normalMatrix * vertex_normal);
    reflection = gl_LightSource[0].halfVector.xyz;
    position = viewMatrix * modelMatrix * vertex;

    ambient = vec4(materialAmbient, 1.0) * gl_LightSource[0].ambient;
    diffuse = vec4(materialDiffuse, 1.0) * gl_LightSource[0].diffuse;
    ambientGlobal = vec4(materialAmbient, 1.0) * gl_LightModel.ambient;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vertex;
}
