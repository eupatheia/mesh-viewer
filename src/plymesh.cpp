//--------------------------------------------------
// Author: JL
// Date: 3 March 2023
// Description: Loads PLY files in ASCII format
//--------------------------------------------------

#include <cstring>
#include "plymesh.h"

using namespace std;
using namespace glm;

namespace agl {

  PLYMesh::PLYMesh(const std::string& filename) {
    _xmin = FLT_MAX;
    _xmax = -FLT_MAX;
    _ymin = FLT_MAX;
    _ymax = -FLT_MAX;
    _zmin = FLT_MAX;
    _zmax = -FLT_MAX;
    load(filename);
  }

  PLYMesh::PLYMesh() {
    _xmin = FLT_MAX;
    _xmax = -FLT_MAX;
    _ymin = FLT_MAX;
    _ymax = -FLT_MAX;
    _zmin = FLT_MAX;
    _zmax = -FLT_MAX;
  }

  void PLYMesh::init() {
    assert(_positions.size() != 0);
    if (_hasTexCoords) {
      initBuffers(&_faces, &_positions, &_normals, &_texCoords);
    } else {
      initBuffers(&_faces, &_positions, &_normals);
    }
  }

  PLYMesh::~PLYMesh() {  }

  bool PLYMesh::load(const std::string& filename) {
    if (_positions.size() != 0) {
      std::cout << "WARNING: Cannot load different files with same PLY mesh\n";
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
    int count = 0;  // number of vertex properties
    sscanf(line, "element vertex %d", &numVertices);
    fgets(line, 1024, file);
    // skip over property definitions
    while (line[0] != 'e') {
      count++;
      fgets(line, 1024, file);
    }
    if (count > 6) {
      _hasTexCoords = 1;
    } else {
      _hasTexCoords = 0;
    }
    int numFaces;
    sscanf(line, "element face %d", &numFaces);
    fgets(line, 1024, file);  // skip "property" line
    fgets(line, 1024, file);  // skip "end_header"
    // read in vertex data
    float x, y, z, nx, ny, nz, s, t;
    for (int i = 0; i < numVertices; i++) {
      fgets(line, 1024, file);
      if (_hasTexCoords) {
        // eight values per vertex
        sscanf(line, "%f %f %f %f %f %f %f %f", &x, &y, &z, &nx, &ny, &nz,
            &s, &t);
        _texCoords.push_back(s);
        _texCoords.push_back(t);
      } else {
        // six values per vertex
        sscanf(line, "%f %f %f %f %f %f", &x, &y, &z, &nx, &ny, &nz);
      }
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

    // Return whether file has UV coordinates
  int PLYMesh::hasUV() const {
    return _hasTexCoords;
  }

  // Normals in this model
  const std::vector<GLfloat>& PLYMesh::texCoords() const {
    return _texCoords;
  }

  void PLYMesh::updateBoundingBox(const float x, const float y,
      const float z) {
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
