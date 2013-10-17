#include "main.h"
using namespace std;

float windowX = 640.0f;
float windowY = 480.0f;

TriangleMesh trig;
Shader shader;

glm::mat4 projectionMatrix, viewMatrix, modelMatrix;
glm::mat3 normalMatrix;
GLfloat materialAmbient[3]    = {1.00, 0.50, 0.00};
GLfloat materialDiffuse[3]    = {0.55, 0.55, 0.55};
GLfloat materialSpecular[3]   = {0.99, 0.99, 0.99};
GLfloat shininess             = 10;
GLfloat constantAttenuation   = 0.1;
GLfloat linearAttenuation     = 0.9;
GLfloat lightPosition[3]      = {0.00, 0.00, 2.00};
GLfloat lightAmbient[3]       = {0.01, 0.01, 0.01};
GLfloat lightDiffuse[3]       = {0.33, 0.33, 0.33};
GLfloat lightSpecular[3]      = {0.50, 0.50, 0.50};
GLfloat lightGlobal[3]        = {0.33, 0.33, 0.33};

GLuint vertex_position_buffer_object;
GLuint vertex_normal_buffer_object;
GLuint vertex_uv_buffer_object;
GLuint textureID;

void cleanup() {
}

void display_handler() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1,1,1);
	shader.Bind();

	// Find the location of our uniform variables in the current shader program
	GLint projectionMatrix_location = glGetUniformLocation(shader.ID(), "projectionMatrix");
	GLint viewMatrix_location = glGetUniformLocation(shader.ID(), "viewMatrix");
	GLint modelMatrix_location = glGetUniformLocation(shader.ID(), "modelMatrix");
	GLint normalMatrix_location = glGetUniformLocation(shader.ID(), "normalMatrix");
	GLint materialAmbient_location = glGetUniformLocation(shader.ID(), "materialAmbient");
	GLint materialDiffuse_location = glGetUniformLocation(shader.ID(), "materialDiffuse");
	GLint materialSpecular_location = glGetUniformLocation(shader.ID(), "materialSpecular");
	GLint lightPosition_location = glGetUniformLocation(shader.ID(), "lightPosition");
	GLint lightAmbient_location = glGetUniformLocation(shader.ID(), "lightAmbient");
	GLint lightDiffuse_location = glGetUniformLocation(shader.ID(), "lightDiffuse");
	GLint lightSpecular_location = glGetUniformLocation(shader.ID(), "lightSpecular");
	GLint lightGlobal_location = glGetUniformLocation(shader.ID(), "lightGlobal");
	GLint shininess_location = glGetUniformLocation(shader.ID(), "shininess");
	GLint constantAttenuation_location = glGetUniformLocation(shader.ID(), "constantAttenuation");
	GLint linearAttenuation_location = glGetUniformLocation(shader.ID(), "linearAttenuation");
	// Pass the current values for our variables to the shader program
	glUniformMatrix4fv(projectionMatrix_location, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(viewMatrix_location, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(modelMatrix_location, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix3fv(normalMatrix_location, 1, GL_FALSE, &normalMatrix[0][0]);
    glUniform3fv(materialAmbient_location, 1, materialAmbient);
    glUniform3fv(materialDiffuse_location, 1, materialDiffuse);
    glUniform3fv(materialSpecular_location, 1, materialSpecular);
    glUniform3fv(lightPosition_location, 1, lightPosition);
    glUniform3fv(lightAmbient_location, 1, lightAmbient);
    glUniform3fv(lightDiffuse_location, 1, lightDiffuse);
    glUniform3fv(lightSpecular_location, 1, lightSpecular);
    glUniform3fv(lightGlobal_location, 1, lightGlobal);
    glUniform1f(shininess_location, shininess);
    glUniform1f(constantAttenuation_location, constantAttenuation);
    glUniform1f(linearAttenuation_location, linearAttenuation);

    // Tell OpenGL we will be using texture variable in the shader
    GLint texture0_location = glGetAttribLocation(shader.ID(), "texture0");
    if (texture0_location != -1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(texture0_location, 0);
    }

    // Tell OpenGL we will be using vertex position variable in the shader
	GLint position_location = glGetAttribLocation(shader.ID(), "vertex_position");
	if (position_location != -1) {
        glEnableVertexAttribArray(position_location);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_position_buffer_object);
        glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

	// Tell OpenGL we will be using vertex uv variable in the shader
	GLint uv_location = glGetAttribLocation(shader.ID(), "vertex_uv");
	if (uv_location != -1) {
        glEnableVertexAttribArray(uv_location);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_uv_buffer_object);
        glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

	// Tell OpenGL we will be using vertex normal variable in the shader
	GLint normal_location = glGetAttribLocation(shader.ID(), "vertex_normal");
	if (normal_location != -1) {
        glEnableVertexAttribArray(normal_location);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_normal_buffer_object);
        glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

	glDrawArrays(GL_TRIANGLES, 0, trig.VertexCount());
	glDisableVertexAttribArray(position_location);
	glDisableVertexAttribArray(uv_location);
	glDisableVertexAttribArray(normal_location);
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

void setup_texture(char *texture_path, GLuint *textureID) {
    if (texture_path == NULL) return;
    unsigned char header[54];
    unsigned int data_pos, width, height, texture_size;
    unsigned char *data;
    // open file
    FILE *file = fopen(texture_path, "rb");
    if (!file) {
        cerr << "couldn't open image" << endl;
        return;
    }
    // validate header
    if ((fread(header, 1, 54, file) != 54) || (header[0] != 'B' || header[1] != 'M')) {
        std::cerr << "not a valid bmp file" << std::endl;
        return;
    }
    // read integers
    data_pos     = *(int*)&(header[0x0A]);
    texture_size = *(int*)&(header[0x22]);
    width        = *(int*)&(header[0x12]);
    height       = *(int*)&(header[0x16]);
    // set defaults if bmp is misformatted
    if (texture_size == 0) texture_size = width * height * 3;
    if (data_pos == 0)     data_pos = 54;
    // read image data
    data = new unsigned char[texture_size];
    fread(data, 1, texture_size, file);
    fclose(file);
    // convert to OpenGL texture
    glGenTextures(1, textureID);
    glBindTexture(GL_TEXTURE_2D, *textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    glEnable(GL_TEXTURE_2D);
}

void setup_vertex_position_buffer_object() {
	glGenBuffers(1, &vertex_position_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_position_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * trig.VertexCount(),
		         &trig.Vertices()[0], GL_STATIC_DRAW);
}

void setup_vertex_uv_buffer_object() {
	glGenBuffers(1, &vertex_uv_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_uv_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * trig.UVs().size(),
		         &trig.UVs()[0], GL_STATIC_DRAW);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
                 &normals[0], GL_STATIC_DRAW);
}

int main(int argc, char **argv) {
	atexit(cleanup);
    cout << "Computer Graphics Assignment 1" << endl;
    char *model_path = NULL;
    char *vertexshader_path = NULL;
    char *fragmentshader_path = NULL;
    char *texture_path = NULL;
    bool use_smoothed_normals;
    if (argc >= 6) {
        texture_path = argv[5];
    }
	if (argc >= 5) {
        model_path = argv[1];
        vertexshader_path = argv[2];
        fragmentshader_path = argv[3];
        use_smoothed_normals = *argv[4] != '0';
	} else {
        cerr << "Usage:" << endl;
		cerr << argv[0]
             << " <model::path> "
             << " <vertex-shader::path> "
             << " <fragment-shader::path> "
             << " <smooth-normals::{0,1}>"
             << " (<texture::path>)"
             << endl;
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
    setup_texture(texture_path, &textureID);
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
