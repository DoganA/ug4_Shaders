#ifndef _rt_H
#define _rt_H

#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

#include <GL/glew.h>
#include <GL/glut.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;

// the Triangle class stores an index to 3 vertices. These are internally stored
// as indices to a vector of vertices (which are Vector3f).
class Triangle;

// The TriangleMesh class actually stores the geometry of an object.
// It stores the vertices and a list of triangles (indices into the vertex list)
class TriangleMesh;

// An output stream, useful for debugging.
// Allows you to stream glm::vec3 objects to the terminal.
ostream & operator << (ostream & stream, const glm::vec3 & obj) {
	stream << obj.x << ' ' << obj.y << ' ' << obj.z << ' ';
	return stream;
}


class Triangle {
    friend class TriangleMesh;

	int _vertex[3]; // indices to the 3 vertices of the triangle

    public:
        Triangle(int v1, int v2, int v3) {
            _vertex[0] = v1;  _vertex[1] = v2;  _vertex[2] = v3;
        }
};

float fmax(float f1,float f2, float f3) {
	float f = f1;
	if (f < f2) f = f2;
	if (f < f3) f = f3;
	return f;
}

float fmin(float f1,float f2, float f3) {
	float f = f1;
	if (f > f2) f = f2;
	if (f > f3) f = f3;
	return f;
}

class TriangleMesh {
	vector <glm::vec3> _vertices; // _vertices stores the vertex positions as glm::vec3
	vector <glm::vec2> _uvs;
	vector <Triangle> _triangles; // _triangle stores the triangles as instances of the Triangle class
	glm::vec3 _min, _max; // These store the bounding values for each axis.

    public:
        TriangleMesh(char * filename) { LoadFile(filename) ;};
        TriangleMesh() {};
        void LoadFile(char * filename);

        int TriangleCount() { return _triangles.size() ;};
        int VertexCount() { return _vertices.size();};

        vector<glm::vec3> &Vertices() { return _vertices; }
};

#endif
