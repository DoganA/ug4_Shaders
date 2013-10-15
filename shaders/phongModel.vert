// Vertex Shader - acts at a per-vertex level
#version 120

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

attribute vec3 vertex_position, vertex_normal;
varying vec3 vertex_color;

void main(void) {
    vec3 N = normalize(gl_NormalMatrix * vertex_normal); // normal
    vec3 L = normalize(vec3(gl_LightSource[0].position)); // light
    vec3 R = gl_LightSource[0].halfVector.xyz; // reflection
    vec3 ambient, specular, diffuse;

    vec3 ka = vec3(0.50, 0.50, 0.50); // ambient reflectance
    vec3 Ia = vec3(1.00, 0.50, 0.00); // ambient light intensity
    ambient = ka * Ia;

    vec3 Ip = vec3(0.50, 0.50, 0.50); // diffuse light intensity
    vec3 kd = vec3(0.85, 0.85, 0.85); // diffuse reflectivity
    float cosTheta = max(dot(L, N), 0.0);
    diffuse = Ip * kd * cosTheta;

    vec3 Is = vec3(0.95, 0.95, 0.95); // specular light intensity
    vec3 ks = vec3(0.99, 0.99, 0.99); // specular reflectance
    float n = 100; // shiny reflectance of object
    float cosAlpha = cosTheta > 0.0 ? max(dot(N, R), 0.0) : 0.0f;
    specular = Is * ks * pow(cosAlpha, n);

    vertex_color = ambient + diffuse + specular;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex_position, 1.0);
}
