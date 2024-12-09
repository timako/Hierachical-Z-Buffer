#ifndef RENDERER_H
#define RENDERER_H

#include "model.h"
#include "shader.h"
#include "camera.h"
#include "framebuffer.h"

class Renderer {
public:
    int width;
    int height;
    Framebuffer framebuffer;
    Shader shader;
    Camera camera;

    Renderer(int w, int h, const Shader& shd, const Camera& cam);

    void render(const Model& model);
private:
    // Helper functions
    Vec3f multiplyMatrixVec(const float matrix[4][4], const Vec3f& v) const;
    void drawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);
};

#endif // RENDERER_H
