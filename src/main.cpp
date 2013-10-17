#include "main.h"
using namespace std;

float windowX = 640.0f;
float windowY = 480.0f;

TriangleMesh trig;
Shader shader;

glm::mat4 projectionMatrix, viewMatrix, modelMatrix;
glm::mat3 normalMatrix;
float materialAmbient[3]    = {1.00, 0.50, 0.00};
float materialDiffuse[3]    = {0.55, 0.55, 0.55};
float materialSpecular[3]   = {0.99, 0.99, 0.99};
float shininess             = 10;
float constantAttenuation   = 0.1;
float linearAttenuation     = 0.9;

unsigned int vertex_position_buffer_object;
unsigned int vertex_normal_buffer_object;
unsigned int vertex_uv_buffer_object;

void cleanup() {
}

void display_handler() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1,1,1);
	shader.Bind();

	// Find the location of our uniform variables in the current shader program
	int projectionMatrix_location = glGetUniformLocation(shader.ID(), "projectionMatrix");
	int viewMatrix_location = glGetUniformLocation(shader.ID(), "viewMatrix");
	int modelMatrix_location = glGetUniformLocation(shader.ID(), "modelMatrix");
	int normalMatrix_location = glGetUniformLocation(shader.ID(), "normalMatrix");
	int materialAmbient_location = glGetUniformLocation(shader.ID(), "materialAmbient");
	int materialDiffuse_location = glGetUniformLocation(shader.ID(), "materialDiffuse");
	int materialSpecular_location = glGetUniformLocation(shader.ID(), "materialSpecular");
	int shininess_location = glGetUniformLocation(shader.ID(), "shininess");
	int constantAttenuation_location = glGetUniformLocation(shader.ID(), "constantAttenuation");
	int linearAttenuation_location = glGetUniformLocation(shader.ID(), "linearAttenuation");
	// Pass the current values for our variables to the shader program
	glUniformMatrix4fv(projectionMatrix_location, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(viewMatrix_location, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(modelMatrix_location, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix3fv(normalMatrix_location, 1, GL_FALSE, &normalMatrix[0][0]);
    glUniform3fv(materialAmbient_location, 1, materialAmbient);
    glUniform3fv(materialDiffuse_location, 1, materialDiffuse);
    glUniform3fv(materialSpecular_location, 1, materialSpecular);
    glUniform1f(shininess_location, shininess);
    glUniform1f(constantAttenuation_location, constantAttenuation);
    glUniform1f(linearAttenuation_location, linearAttenuation);

    // Tell OpenGL we will be using vertex position variable in the shader
	int position_location = glGetAttribLocation(shader.ID(), "vertex_position");
	if (position_location != -1) {
        glEnableVertexAttribArray(position_location);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_position_buffer_object);
        glVertexAttribPointer(
            position_location,
            3,                 // number of elements per vertex, here (x,y,z)
            GL_FLOAT,          // the type of each element
            GL_FALSE,          // take our values as-is
            0,                 // no extra data between each position
            0                  // offset of first element
        );
    }

	// Tell OpenGL we will be using vertex uv variable in the shader
	int uv_location = glGetAttribLocation(shader.ID(), "vertex_uv");
	if (uv_location != -1) {
        glEnableVertexAttribArray(uv_location);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_uv_buffer_object);
        glVertexAttribPointer(
            uv_location,
            2,                 // number of elements per vertex, here (u,v)
            GL_FLOAT,          // the type of each element
            GL_FALSE,          // take our values as-is
            0,                 // no extra data between each position
            0                  // offset of first element
        );
    }

	// Tell OpenGL we will be using vertex normal variable in the shader
	int normal_location = glGetAttribLocation(shader.ID(), "vertex_normal");
	if (normal_location != -1) {
        glEnableVertexAttribArray(normal_location);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_normal_buffer_object);
        glVertexAttribPointer(
            normal_location,
            3,                 // number of elements per vertex, here (x,y,z)
            GL_FLOAT,          // the type of each element
            GL_FALSE,          // take our values as-is
            0,                 // no extra data between each position
            0                  // offset of first element
        );
    }

	glDrawArrays(GL_TRIANGLES, 0, trig.VertexCount());
	glDisableVertexAttribArray(position_location);
	shader.Unbind();
	glFlush();
}

glm::mat4 _get_projectionMatrix(void) {
    return glm::ortho(-windowX * 0.5f,
                       windowX * 0.5f,
                      -windowY * 0.5f,
                       windowY * 0.5f,
                      -1.0f, 400.0f);
}
glm::mat4 _get_viewMatrix(void) {
    return glm::translate(glm::mat4(1.0f),glm::vec3(-50.0f,-50.0f,-300.0f));
}
glm::mat3 _get_normalMatrix(void) {
    return glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
}
glm::mat4 _get_modelMatrix(void) {
    return glm::translate(glm::mat4(1.0f),glm::vec3(0.0f));
}

void keyboard_handler(unsigned char key, int x, int y) {
    glm::vec3 translation = glm::vec3(0, 0, 0);
    glm::vec3 rotation = glm::vec3(0, 0, 0);
    switch (key) {
        case 'q': translation = glm::vec3( 1, 0, 0); break;
        case 'w': translation = glm::vec3(-1, 0, 0); break;
        case 'e': translation = glm::vec3( 0, 1, 0); break;
        case 'r': translation = glm::vec3( 0,-1, 0); break;
        case 't': translation = glm::vec3( 0, 0, 1); break;
        case 'y': translation = glm::vec3( 0, 0,-1); break;
        case 'a': rotation = glm::vec3( 1, 0, 0); break;
        case 's': rotation = glm::vec3(-1, 0, 0); break;
        case 'd': rotation = glm::vec3( 0, 1, 0); break;
        case 'f': rotation = glm::vec3( 0,-1, 0); break;
        case 'g': rotation = glm::vec3( 0, 0, 1); break;
        case 'h': rotation = glm::vec3( 0, 0,-1); break;
        case ' ': viewMatrix = _get_viewMatrix(); break;
    }
    if (translation.x != 0 || translation.y != 0 || translation.z != 0) {
        viewMatrix = glm::translate(viewMatrix, translation);
    } else if (rotation.x != 0 || rotation.y != 0 || rotation.z != 0) {
        viewMatrix = glm::rotate(viewMatrix, 1.0f, rotation);
    }
    normalMatrix = _get_normalMatrix();
    display_handler();
}

void setup_vertex_position_buffer_object() {
	glGenBuffers(1, &vertex_position_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_position_buffer_object);
	glBufferData(
        GL_ARRAY_BUFFER,                        // the target buffer object
		sizeof(glm::vec3) * trig.VertexCount(), // size in bytes for the data
		&trig.Vertices()[0],                    // pointer to the array of data
		GL_STATIC_DRAW);
	cout << "vertex_position_buffer_object generated!" << endl;
}

void setup_vertex_uv_buffer_object() {
	glGenBuffers(1, &vertex_uv_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_uv_buffer_object);
	glBufferData(
        GL_ARRAY_BUFFER,                       // the target buffer object
		sizeof(glm::vec2) * trig.UVs().size(), // size in bytes for the data
		&trig.UVs()[0],                        // pointer to the array of data
		GL_STATIC_DRAW);
	cout << "vertex_uv_buffer_object generated!" << endl;
}

void setup_vertex_normal_buffer_object(bool smoothed) {
    vector<glm::vec3> vertices = trig.Vertices();
    vector<glm::vec3> normals;
    if (smoothed) {
        // initialize map of normals to zero
        map< vector<double>, vector<double> > normal_map;
        for (int i = 0; i < vertices.size(); i++) {
            vector<double> zeros;
            zeros.push_back(0.0);
            zeros.push_back(0.0);
            zeros.push_back(0.0);
            normal_map[to_vector(vertices[i])] = zeros;
        }
        for (int i = 0; i < vertices.size(); i += 3) {
            // get vertices of this triangle
            glm::vec3 v1 = vertices[i];
            glm::vec3 v2 = vertices[i + 1];
            glm::vec3 v3 = vertices[i + 2];
            vector<double> v1_key = to_vector(v1);
            vector<double> v2_key = to_vector(v2);
            vector<double> v3_key = to_vector(v3);
            // compute face normal
            glm::vec3 face_normal = glm::cross(v3 - v2, v1 - v2);
            // get the old vertex normal
            glm::vec3 v1_old = to_vec3(normal_map[v1_key]);
            glm::vec3 v2_old = to_vec3(normal_map[v2_key]);
            glm::vec3 v3_old = to_vec3(normal_map[v3_key]);
            // replace the old value with the new value
            normal_map.erase(v1_key);
            normal_map.erase(v2_key);
            normal_map.erase(v3_key);
            normal_map[v1_key] = to_vector(glm::normalize(v1_old + face_normal));
            normal_map[v2_key] = to_vector(glm::normalize(v2_old + face_normal));
            normal_map[v3_key] = to_vector(glm::normalize(v3_old + face_normal));
        }
        // convert the map of normals to a vector of normals
        for (int i = 0; i < vertices.size(); i++) {
            normals.push_back(to_vec3(normal_map[to_vector(vertices[i])]));
        }
    } else {
        for (int i = 0; i < vertices.size(); i += 3) {
            // get vertices of this triangle
            glm::vec3 v1 = vertices[i];
            glm::vec3 v2 = vertices[i + 1];
            glm::vec3 v3 = vertices[i + 2];
            // compute face normal
            glm::vec3 face_normal = glm::cross(v3 - v2, v1 - v2);
            normals.push_back(glm::normalize(face_normal));
            normals.push_back(glm::normalize(face_normal));
            normals.push_back(glm::normalize(face_normal));
        }
    }
    glGenBuffers(1, &vertex_normal_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_normal_buffer_object);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(glm::vec3) * normals.size(),
        &normals[0],
        GL_STATIC_DRAW);
    cout << "vertex_normal_buffer_object generated!" << endl;
}

int main(int argc, char **argv) {
	atexit(cleanup);
    cout << "Computer Graphics Assignment 1" << endl;
    char *model_path;
    char *vertexshader_path;
    char *fragmentshader_path;
    bool use_smoothed_normals;
	if (argc == 5) {
        model_path = argv[1];
        vertexshader_path = argv[2];
        fragmentshader_path = argv[3];
        use_smoothed_normals = *argv[4] != '0';
	} else {
        cerr << "Usage:" << endl;
		cerr << argv[0] << " <model> <vertex-shader> <fragment-shader> <smooth-normals>" << endl;
		exit(1);
	}
	// initialise OpenGL
	glutInit(&argc, argv);
	glutInitWindowSize(windowX, windowY);
	glutCreateWindow("CG-CW1");
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);
    // set display/keyboard callbacks
	glutDisplayFunc(display_handler);
    glutKeyboardFunc(keyboard_handler);
	// initialise the OpenGL Extension Wrangler library for VBOs
	GLenum err = glewInit();
	if (err != GLEW_OK){
		cout << "Error!" << endl;
		exit(1);
	}
	if (!GLEW_VERSION_2_1) {
		cout << "Error 2.1!" << endl;
		exit(1);
	}
	// create shader, prepare data for OpenGL
    trig.LoadFile(model_path);
	shader.Init(vertexshader_path, fragmentshader_path);
	setup_vertex_position_buffer_object();
	setup_vertex_uv_buffer_object();
    setup_vertex_normal_buffer_object(use_smoothed_normals);
	// set up camera and object transformation matrices
	projectionMatrix = _get_projectionMatrix();
	viewMatrix = _get_viewMatrix();
	modelMatrix = _get_modelMatrix();
    normalMatrix = _get_normalMatrix();
	glutMainLoop();
}
