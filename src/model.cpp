#include "model.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// BoundingBox Implementation

BoundingBox::BoundingBox() {
    // Initialize min and max to extreme values
    min = Vec3f(std::numeric_limits<float>::max(),
               std::numeric_limits<float>::max(),
               std::numeric_limits<float>::max());
    max = Vec3f(std::numeric_limits<float>::lowest(),
               std::numeric_limits<float>::lowest(),
               std::numeric_limits<float>::lowest());
}

void BoundingBox::update(const Vec3f& vertex) {
    if (vertex.x < min.x) min.x = vertex.x;
    if (vertex.y < min.y) min.y = vertex.y;
    if (vertex.z < min.z) min.z = vertex.z;

    if (vertex.x > max.x) max.x = vertex.x;
    if (vertex.y > max.y) max.y = vertex.y;
    if (vertex.z > max.z) max.z = vertex.z;
}

// Model Implementation

Model::Model() {
    // Constructor implementation (if needed)
}

Model::~Model() {
    // Destructor implementation (if needed)
}

bool Model::loadFromOBJ(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filename << std::endl;
        return false;
    }

    std::string line;
    // Temporary variables for parsing
    std::string prefix;
    while (std::getline(infile, line)) {
        // Remove carriage return character for Windows compatibility
        // std::cout << "Parsing line: " << line << std::endl; // Debug statement

        if(line.empty()) {
            continue;
        }

        if (!line.empty() && line[line.size() - 1] == '\r') {
            line.pop_back();
        }

        std::istringstream iss(line);
        iss >> prefix;

        if (prefix == "v") {
            // Vertex position
            float x, y, z;
            iss >> x >> y >> z;
            vertices.emplace_back(x, y, z);
            // std::cout << "Parsed vertex: " << vertices.back() << std::endl; // Debug statement
            bbox.update(Vec3f(x, y, z));
        }
        else if (prefix == "vt") {
            // Texture coordinate
            float u, v;
            iss >> u >> v;
            texcoords.emplace_back(u, v);
            // std::cout << "Parsed texcoord: " << texcoords.back() << std::endl; // Debug statement
        }
        else if (prefix == "vn") {
            // Normal vector
            float nx, ny, nz;
            iss >> nx >> ny >> nz;
            normals.emplace_back(nx, ny, nz);
            // std::cout << "Parsed normal: " << normals.back() << std::endl; // Debug statement
        }
        else if (prefix == "f") {
            // Face
            Face face;
            std::string vertexStr;
            int vertexCount = 0;

            while (iss >> vertexStr && vertexCount < 3) { // Assuming triangles
                std::istringstream viss(vertexStr);
                std::string vIdxStr, vtIdxStr, vnIdxStr;
                int vIdx = 0, vtIdx = 0, vnIdx = 0;

                // Parse the vertex string (e.g., "1/2/3")
                size_t firstSlash = vertexStr.find('/');
                size_t secondSlash = vertexStr.find('/', firstSlash + 1);

                if (firstSlash == std::string::npos) {
                    // Only vertex index is present
                    vIdx = std::stoi(vertexStr);
                }
                else if (secondSlash == std::string::npos) {
                    // Vertex and texture indices
                    vIdxStr = vertexStr.substr(0, firstSlash);
                    vtIdxStr = vertexStr.substr(firstSlash + 1);
                    vIdx = std::stoi(vIdxStr);
                    vtIdx = std::stoi(vtIdxStr);
                }
                else {
                    // Vertex, texture, and normal indices
                    vIdxStr = vertexStr.substr(0, firstSlash);
                    vtIdxStr = vertexStr.substr(firstSlash + 1, secondSlash - firstSlash - 1);
                    vnIdxStr = vertexStr.substr(secondSlash + 1);
                    vIdx = std::stoi(vIdxStr);
                    if (!vtIdxStr.empty()) {
                        vtIdx = std::stoi(vtIdxStr);
                    }
                    if (!vnIdxStr.empty()) {
                        vnIdx = std::stoi(vnIdxStr);
                    }
                }

                // OBJ indices are 1-based, adjust to 0-based
                face.vertices[vertexCount].v = (vIdx > 0) ? (vIdx - 1) : (vertices.size() + vIdx);
                face.vertices[vertexCount].vt = (vtIdx > 0) ? (vtIdx - 1) : (texcoords.size() + vtIdx);
                face.vertices[vertexCount].vn = (vnIdx > 0) ? (vnIdx - 1) : (normals.size() + vnIdx);
                vertexCount++;
            }

            if (vertexCount != 3) {
                std::cerr << "Non-triangular face detected. Only triangles are supported." << std::endl;
                // Optionally, implement triangulation for polygons with more vertices
                continue;
            }

            faces.emplace_back(face);
        }
        // Ignore other prefixes (e.g., "o", "g", "s", "usemtl", etc.)
    }

    infile.close();

    std::cout << "Total normals parsed: " << normals.size() << std::endl; // Debug statement
    std::cout << "Total vertices parsed: " << vertices.size() << std::endl; // Debug statement
    std::cout << "Total texcoords parsed: " << texcoords.size() << std::endl; // Debug statement
    std::cout << "Total faces parsed: " << faces.size() << std::endl; // Debug statement

    return true;
}



void Model::computeBoundingBox() {
    // Already computed during loading in this implementation
    // If not, iterate over vertices to compute min and max
    if (vertices.empty()) {
        std::cerr << "No vertices available to compute bounding box." << std::endl;
        return;
    }

    BoundingBox tempBox;
    for (const auto& vertex : vertices) {
        tempBox.update(vertex);
    }

    bbox = tempBox;
    center = (bbox.min + bbox.max) * 0.5f;
}

void Model::normalizeToUnitCube() {
    computeBoundingBox();
    center = (bbox.min + bbox.max) * 0.5f;
    for(auto& vertex : vertices) {
        vertex -= center;
    }
    float maxExtent = 0.0f;
    Vec3f extents = (bbox.max - bbox.min) * 0.5f;
    maxExtent = std::max({ extents.x, extents.y, extents.z });
    if(maxExtent == 0.0f) {
        
        std::cerr << "Warning: Model has zero size in all dimensions. Scaling skipped." << std::endl;
        return;
    }
    float scale = 1.0f / maxExtent;
    for(auto& vertex : vertices) {
        vertex *= scale;
    }
    computeBoundingBox();
    center = (bbox.min + bbox.max) * 0.5f;
    computeNormals(); 
    // for(int i = 0; i < normals.size(); i++){
    //     std::cout << "normal: " << normals[i] << std::endl;
    // }
}

void Model::computeNormals(){
    vNormals.resize(vertices.size(), Vec3f(0.0f, 0.0f, 0.0f));
    fNormals.resize(faces.size(), Vec3f(0.0f, 0.0f, 0.0f));
    std::vector<int> count(vertices.size(), 0);
    if (normals.empty()) {
        std::cerr << "No normals found. Computing vertex normals..." << std::endl;
        uint faceSize = faces.size();
        for (uint j = 0; j< faceSize; j++) {
            Face face = faces[j];
            const Vec3f& v0 = vertices[face.vertices[0].v];
            const Vec3f& v1 = vertices[face.vertices[1].v];
            const Vec3f& v2 = vertices[face.vertices[2].v];
            Vec3f edge1 = v1 - v0;
            Vec3f edge2 = v2 - v0;
            Vec3f faceNormal = edge1.cross(edge2);
            fNormals[j] = faceNormal;
            if (faceNormal.magnitude() == 0.0f) {
                std::cerr << "Degenerate face detected. Skipping normal computation for this face." << std::endl;
                continue;
            }
            for(int i = 0; i < 3; i++){
                vNormals[face.vertices[i].v] += faceNormal;
                count[face.vertices[i].v]++;
            }
        }
        for(size_t i = 0; i < vNormals.size(); i++){
            if(count[i] > 0){
                vNormals[i] /= count[i];
            }
        }

        for (size_t i = 0; i < vNormals.size(); i++) {
            if(vNormals[i].magnitude() == 0.0f){
                std::cerr << "Vertex " << i << " has a zero normal. Assigning default normal." << std::endl;
                vNormals[i] = Vec3f(0.0f, 1.0f, 0.0f); // Assigning a default normal
                continue;
            }
            try
            {
                vNormals[i].normalize();
            }
            catch (const std::runtime_error& e) {
                std::cerr << "Error normalizing normal for vertex " << i << ": " << e.what() << std::endl;
                vNormals[i] = Vec3f(0.0f, 1.0f, 0.0f); // Assign a default normal
            }
        }
    }
    else{
        uint faceSize = faces.size();
        for (uint j = 0; j< faceSize; j++) {
            Face face = faces[j];
            const Vec3f& v0 = vertices[face.vertices[0].v];
            const Vec3f& v1 = vertices[face.vertices[1].v];
            const Vec3f& v2 = vertices[face.vertices[2].v];
            Vec3f edge1 = v1 - v0;
            Vec3f edge2 = v2 - v0;
            Vec3f faceNormal = edge1.cross(edge2);
            fNormals[j] = faceNormal;
        }

        for (const auto& face : faces) {
            for(int i = 0; i < 3; i++){
                vNormals[face.vertices[i].v] += normals[face.vertices[i].vn];
                count[face.vertices[i].v]++;
            }
        }
        for(size_t i = 0; i < vNormals.size(); i++){
            if(count[i] > 0){
                vNormals[i] /= count[i];
                vNormals[i].normalize();
            }
        }
    }
}
