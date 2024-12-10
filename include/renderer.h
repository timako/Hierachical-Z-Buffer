#ifndef RENDERER_H
#define RENDERER_H

#include "model.h"
#include "shader.h"
#include "camera.h"
#include "framebuffer.h"
#include "vector"

class Renderer {
public:
    int width;
    int height;
    Framebuffer *framebuffer;
    Shader shader;
    Camera camera;
    enum ZBufferMethod {
        Simple,
        ScanLine, 
        SimpleHierarchical,
        OctreeHierarchical
    };
    ZBufferMethod zBufferMethod = ZBufferMethod::Simple; 

    Renderer(int w, int h, const Shader& shd, const Camera& cam);

    void render(const Model& model);
private:
    // Helper functions
    Vec3f multiplyMatrixVec(const float matrix[4][4], const Vec3f& v) const;
    void drawTriangle(const std::vector<Vertex>);
    void drawTriangleWithNormal(const std::vector<Vertex>, Vec3f normal); 
};

#endif // RENDERER_H
