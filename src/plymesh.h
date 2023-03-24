//--------------------------------------------------
// Author: JL
// Date: 3 March 2023
// Description: header file for plymesh.cpp
//--------------------------------------------------

#ifndef plymeshmodel_H_
#define plymeshmodel_H_

#include "agl/aglm.h"
#include "agl/mesh/triangle_mesh.h"

namespace agl {
  class PLYMesh : public TriangleMesh {

   public:

    PLYMesh(const std::string& filename);
    PLYMesh();

    virtual ~PLYMesh();

    // Initialize this object with the given file
    // Returns true if successful, false otherwise.
    bool load(const std::string& filename);

    // Return the minimum point of the axis-aligned bounding box
    glm::vec3 minBounds() const;

    // Return the maximum point of the axis-aligned bounding box
    glm::vec3 maxBounds() const;

    // Return number of vertices in this model
    int numVertices() const;

    // Return number of faces in this model
    int numTriangles() const;

    // Positions in this model
    const std::vector<GLfloat>& positions() const;

    // Normals in this model
    const std::vector<GLfloat>& normals() const;

    // Face indices in this model
    const std::vector<GLuint>& indices() const;

    // Return whether file has UV coordinates
    int hasUV() const;

    // UVs in this model
    const std::vector<GLfloat>& texCoords() const;

   protected:
    void init();
    // helper function to update bounding box values
    void updateBoundingBox(const float x, const float y, const float z);

   protected:
    // every 3 numbers is a vertex position (x, y, z)
    std::vector<GLfloat> _positions;
    // every 3 numbers is a set of vertex normals (x, y, z)
    std::vector<GLfloat> _normals;
    // every 3 numbers defines a triangle's vertices
    std::vector<GLuint> _faces;
    // every two numbers is a uv
    std::vector<GLfloat> _texCoords;
    float _xmin;
    float _xmax;
    float _ymin;
    float _ymax;
    float _zmin;
    float _zmax;
    int _hasTexCoords;  // 1 if file has UV coords, else 0;
  };
}

#endif
