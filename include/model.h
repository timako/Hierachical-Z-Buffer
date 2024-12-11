#ifndef MODEL_H
#define MODEL_H

#include <vector.h>
#include <vector>
#include <string>
#include <array>

// Structure to hold indices for a face
struct Face {
    // Each face can be a triangle or a polygon.
    // Here, we'll assume triangles for simplicity.
    // Each vertex of the face has an index for position, texture, and normal.
    struct VertexIndices {
        int v;  // Vertex position index
        int vt; // Texture coordinate index
        int vn; // Normal index
    };

    // Using three vertices for a triangle
    std::array<VertexIndices, 3> vertices;
};

// Structure to represent a bounding box
struct BoundingBox {
    Vec3f min;
    Vec3f max;

    BoundingBox();
    void update(const Vec3f& vertex);
};

// The main Model class
class Model {
public:
    // Data
    std::vector<Vec3f> vertices;      // List of vertex positions
    std::vector<Vec3f> normals;       // List of normals
    std::vector<Vec2f> texcoords;     // List of texture coordinates
    std::vector<Face> faces;          // List of faces
    std::vector<Vec3f> vNormals;      // List of vertex normals
    std::vector<Vec3f> fNormals;      // List of face normals
    BoundingBox bbox;                 // Bounding box
    Vec3f center; 

    // Constructors
    Model();
    ~Model();

    // Methods
    bool loadFromOBJ(const std::string& filename);
    void computeBoundingBox();
    
    // New Method
    void normalizeToUnitCube();
    void computeNormals();

};


#endif // MODEL_H
