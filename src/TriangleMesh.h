#ifndef _triangle_H
#define _triangle_H

#include <vector>
#include <cmath>
#include <fstream>
#include <cstring>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "utils.h"

using namespace std;

class Triangle;
class TriangleMesh;

class Triangle {
    friend class TriangleMesh;
	int _vertex[3];

    public:
        Triangle(int v1, int v2, int v3) { _vertex[0] = v1;  _vertex[1] = v2;  _vertex[2] = v3; }
};

class TriangleMesh {
	vector <glm::vec3> _vertices;
	vector <glm::vec2> _uvs;
	vector <Triangle> _triangles;
	glm::vec3 _min, _max;

    public:
        TriangleMesh(char * filename) { LoadFile(filename) ;};
        TriangleMesh() {};
        void LoadFile(char * filename);
        int TriangleCount() { return _triangles.size() ;};
        int VertexCount() { return _vertices.size();};
        vector<glm::vec3> &Vertices() { return _vertices; }
        vector<glm::vec2> &UVs() { return _uvs; }
};

#endif
