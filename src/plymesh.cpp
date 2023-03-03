//--------------------------------------------------
// Author: JL
// Date: 2 March 2023
// Description: Loads PLY files in ASCII format
//--------------------------------------------------

#include "plymesh.h"

using namespace std;
using namespace glm;

namespace agl {

  PLYMesh::PLYMesh(const std::string& filename) {
    float _xmin = FLT_MAX;
    float _xmax = -FLT_MAX;
    float _ymin = FLT_MAX;
    float _ymax = -FLT_MAX;
    float _zmin = FLT_MAX;
    float _zmax = -FLT_MAX;
    load(filename);
  }

  PLYMesh::PLYMesh() {
    float _xmin = FLT_MAX;
    float _xmax = -FLT_MAX;
    float _ymin = FLT_MAX;
    float _ymax = -FLT_MAX;
    float _zmin = FLT_MAX;
    float _zmax = -FLT_MAX;
  }

  void PLYMesh::init() {
    assert(_positions.size() != 0);
    initBuffers(&_faces, &_positions, &_normals);
  }

  PLYMesh::~PLYMesh() {  }

  bool PLYMesh::load(const std::string& filename) {
    if (_positions.size() != 0) {
        std::cout << "WARNING: Cannot load different files with the same PLY mesh\n";
        return false;
    }
    FILE * file = fopen(filename.c_str(), "r");
    if (file == NULL) {
      printf("Error: could not open file %s\n", filename.c_str());
      return false;
    }
    char line[1024];
    fgets(line, 1024, file);
    if (strcmp(line, "ply\n") != 0) {
      printf("Error: %s not ply file format\n", filename.c_str());
      fclose(file);
      return false;
    }
    // skip format and comment lines
    while (line[0] != 'e') {
      fgets(line, 1024, file);
    }
    int numVertices;
    sscanf(line, "element vertex %d", &numVertices);
    fgets(line, 1024, file);
    // skip over property definitions
    while (line[0] != 'e') {
      fgets(line, 1024, file);
    }
    int numFaces;
    sscanf(line, "element face %d", &numFaces);
    fgets(line, 1024, file);  // skip "property" line
    fgets(line, 1024, file);  // skip "end_header"
    // read in vertex data
    float x, y, z, nx, ny, nz;
    for (int i = 0; i < numVertices; i++) {
      fgets(line, 1024, file);
      // six values per vertex
      sscanf(line, "%f %f %f %f %f %f", &x, &y, &z, &nx, &ny, &nz);
      // update bounding box values
      updateBoundingBox(x, y, z);
      _positions.push_back(x);
      _positions.push_back(y);
      _positions.push_back(z);
      _normals.push_back(nx);
      _normals.push_back(ny);
      _normals.push_back(nz);
    }
    // read in polygon data
    int a, b, c;
    for (int i = 0; i < numFaces; i++) {
      fgets(line, 1024, file);
      // "3" plus 3 vertex indices
      sscanf(line, "3 %d %d %d", &a, &b, &c);
      _faces.push_back(a);
      _faces.push_back(b);
      _faces.push_back(c);
    }
    fclose(file);
    return true;
  }

  glm::vec3 PLYMesh::minBounds() const {
    return glm::vec3(_xmin, _ymin, _zmin);
  }

  glm::vec3 PLYMesh::maxBounds() const {
    return glm::vec3(_xmax, _ymax, _zmax);
  }

  int PLYMesh::numVertices() const {
    return _positions.size() / 3;
  }

  int PLYMesh::numTriangles() const {
    return _faces.size() / 3;
  }

  const std::vector<GLfloat>& PLYMesh::positions() const {
    return _positions;
  }

  const std::vector<GLfloat>& PLYMesh::normals() const {
    return _normals;
  }

  const std::vector<GLuint>& PLYMesh::indices() const {
    return _faces;
  }

  void PLYMesh::updateBoundingBox(const float x, const float y, const float z) {
    if (x > _xmax) {
      _xmax = x;
    }
    if (x < _xmin) {
      _xmin = x;
    }
    if (y > _ymax) {
      _ymax = y;
    }
    if (y < _ymin) {
      _ymin = y;
    }
    if (z > _zmax) {
      _zmax = z;
    }
    if (z < _zmin) {
      _zmin = z;
    }
  }
}
