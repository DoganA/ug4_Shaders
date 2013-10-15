#include "demo1.h"
#include "Shader.h"
#include <map>

using namespace std;
float windowX = 640.0f;
float windowY = 480.0f;
TriangleMesh trig;
Shader shader;
glm::mat4 projectionMatrix;
glm::mat4 viewMatrix;
glm::mat4 modelMatrix;

unsigned int vbo; // vertex position buffer object
unsigned int nbo; // vertex normal buffer object

void cleanup() {
}

// This function allows you to draw pixels onto the display by using GL_POINTS.
// Drawn every time an update is required.
// The idea with this example function is the following:
//   1) Clear the screen so we can draw a new frame
//   2) Calculate the vertex points for each triangle and draw them
//   3) Flush the pipeline so that the instructions we gave are performed.
void DemoDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,1,1);
	shader.Bind();

	// Find the location of our uniform variables in the current shader program
	int projectMatrixLocation = glGetUniformLocation(shader.ID(), "projectionMatrix");
	int viewMatrixLocation = glGetUniformLocation(shader.ID(), "viewMatrix");
	int modelMatrixLocation = glGetUniformLocation(shader.ID(), "modelMatrix");
	// Pass the current values for our variables to the shader program
	glUniformMatrix4fv(projectMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	// Find the location for our vertex position variable
	int positionLocation = glGetAttribLocation(shader.ID(), "in_position");
	if (positionLocation == -1) {
		cout << "Could not bind attribute in_position" << endl;
		return;
	}
	// Tell OpenGL we will be using vertex position variable in the shader
	glEnableVertexAttribArray(positionLocation);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		positionLocation,  // attribute (location of the in_position variable in our shader program)
		3,                 // number of elements per vertex, here (x,y,z)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);

	// Find the location for our vertex-normal variable
	int normalLocation = glGetAttribLocation(shader.ID(), "in_normal");
	if (normalLocation == -1) {
		cout << "Could not bind attribute in_normal" << endl;
		return;
	}
	// Tell OpenGL we will be using vertex normal variable in the shader
	glEnableVertexAttribArray(normalLocation);
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glVertexAttribPointer(
		normalLocation,    // attribute (location of the in_normal variable in our shader program)
		3,                 // number of elements per vertex, here (x,y,z)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);
	glDrawArrays(GL_TRIANGLES, 0, trig.VertexCount());
	glDisableVertexAttribArray(positionLocation);
	shader.Unbind();
	glFlush();
}

void DemoKeyboardHandler(unsigned char key, int x, int y) {
    if(key == 'm') {
        cout << "Mouse location: " << x << " " << y << endl;
    }
    cout << "Key pressed: " << key << endl;
}

// create a new Vertex Buffer Object, bind it and stream data to it
void SetupVBO() {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
        GL_ARRAY_BUFFER,                        // the target buffer object
		sizeof(glm::vec3) * trig.VertexCount(), // size in bytes for the data
		&trig.Vertices()[0],                    // pointer to the array of data
		GL_STATIC_DRAW);
	cout << "VBO generated!" << endl;
}

vector<double> to_vector(glm::vec3 v_vec3) {
    vector<double> v_vector;
    v_vector.push_back(v_vec3.x);
    v_vector.push_back(v_vec3.y);
    v_vector.push_back(v_vec3.z);
    return v_vector;
}

glm::vec3 to_vec3(vector<double> v_vector) {
    return glm::vec3(v_vector[0], v_vector[1], v_vector[2]);
}

void SetupNBO() {
    vector<glm::vec3> vertices = trig.Vertices();
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
        glm::vec3 face_normal = glm::cross(v3 - v1, v2 - v1);
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
    vector<glm::vec3> normals;
    for (int i = 0; i < vertices.size(); i++) {
        normals.push_back(to_vec3(normal_map[to_vector(vertices[i])]));
    }
    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(glm::vec3) * normals.size(),
        &normals[0],
        GL_STATIC_DRAW);
    cout << "NBO generated!" << endl;
}

int main(int argc, char **argv) {
	atexit(cleanup);
    cout << "Computer Graphics Assignment 1 Demo Program" << endl;
    char *model_path;
    char *vertexshader_path;
    char *fragmentshader_path;
	if (argc == 4) {
        model_path = argv[1];
        vertexshader_path = argv[2];
        fragmentshader_path = argv[3];
	} else {
        cerr << "Usage:" << endl;
		cerr << argv[0] << " <model> <vertex-shader> <fragment-shader>" << endl;
		exit(1);
	}
	// initialise OpenGL
	glutInit(&argc, argv);
	glutInitWindowSize(windowX, windowY);
	glutCreateWindow("CG-CW1");
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    // set display/keyboard callbacks
	glutDisplayFunc(DemoDisplay);
    glutKeyboardFunc(DemoKeyboardHandler);
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
	SetupVBO();
    SetupNBO();
	// set up camera and object transformation matrices
	projectionMatrix = glm::ortho(-windowX*0.5f, windowX*0.5f, -windowY*0.5f,  windowY*0.5f, -1.0f, 400.0f);
	viewMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(-50.0f,-50.0f,-10.0f));
	modelMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f));
	glutMainLoop();
}

// This function loads an obj format file This is a utility and should not have
// to be modified for teapot.obj (the assignment).  Note that this function does
// not load the normals for the mesh.  These are not provided in the teapot.obj
// file, you must compute these yourself
void TriangleMesh::LoadFile(char * filename) {
	FILE *file = fopen(filename, "r");
	if(file  == NULL){
		cout << "Can't open file " << filename << endl;
		return;
	}
	vector<unsigned int> vertexIndices, uvIndices;
	vector<glm::vec3> tempVertices;
	vector<glm::vec2> tempUVs;
	_max.x =-10000; _max.y =-10000; _max.z =-10000;
	_min.x =10000; _min.y =10000; _min.z =10000;
	glm::vec3 averageVertex(0.0f);
	//Read in .obj
	while(1){
		char lineHeader[128];
		//read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if(res == EOF){
			break;
		}
		if(strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tempVertices.push_back(vertex);
			averageVertex += vertex;
			if (vertex.x > _max.x) _max.x = vertex.x;
			if (vertex.y > _max.y) _max.y = vertex.y;
			if (vertex.z > _max.z) _max.z = vertex.z;
			if (vertex.x < _min.x) _min.x = vertex.x;
			if (vertex.y < _min.y) _min.y = vertex.y;
			if (vertex.z < _min.z) _min.z = vertex.z;
		}
		else if(strcmp(lineHeader, "vt" ) == 0){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			tempUVs.push_back(uv);
		}
		else if(strcmp(lineHeader, "f" ) == 0){
			string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3];
			int matches = fscanf(file, "%d/%d %d/%d %d/%d\n",&vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2] );
			if(matches != 6){
				cout << "Can't be read by simple parser!" << endl;
				return;
			}
			Triangle trig(vertexIndex[0],vertexIndex[1],vertexIndex[2]);
			_triangles.push_back(trig);
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
		}
	}
	//Arrange in triangle order to fit with OpenGL's glDrawArrays(...) function.
	//i.e. _vertices[0],_vertices[1], and _vertices[2] are the first triangle,
	//_vertices[3],_vertices[4], and _vertices[5] are the second etc.
	//Note this invalidates the Triangle structures in _triangles.  The Triangle class
	//is used to help with creating an array of indexes for glDrawElements(...); if you want to use
	//this instead of reordering the vertices
	_vertices.resize(vertexIndices.size());
	_uvs.resize(uvIndices.size());
	for(unsigned int i = 0; i < vertexIndices.size(); ++i){
		_vertices[i] = tempVertices[vertexIndices[i]-1];
		_uvs[i] = tempUVs[uvIndices[i]-1];
	}
	float range;
	if (_max.x-_min.x > _max.y-_min.y){
		range = _max.x-_min.x;
	}
	else{
		range = _max.y-_min.y;
	}
	averageVertex /= _vertices.size();
	for (int i = 0; i < _vertices.size(); i++)
	{
		_vertices[i] = (_vertices[i]-averageVertex)/range*400.0f;
	}
	cout << "Number of triangles: " << _triangles.size() << ", number of vertices: " << _vertices.size() << endl;
};
