#include "matrix.h"
#include "renderer.h"
#include <algorithm>
#include <cmath>

// Constructor
Renderer::Renderer(int w, int h, const Shader& shd, const Camera& cam)
    : width(w), height(h), framebuffer(w, h), shader(shd), camera(cam) {}

void Renderer::render(const Model& model) {
    // Clear framebuffer
    framebuffer.clear(Color(0.1, 0.1, 0.1));

    // Get View and Projection matrices
    Mat4x4 viewMatrix;
    camera.getViewMatrix(viewMatrix);

    Mat4x4 projectionMatrix;
    camera.getProjectionMatrix(projectionMatrix);

    std::cout << "projmat" << projectionMatrix; 

    // Iterate over all faces
    for (const auto& face : model.faces) {
        Vertex vertices[3];
        for (int i = 0; i < 3; ++i) {
            const Face::VertexIndices& idx = face.vertices[i];
            vertices[i].position = model.vertices[idx.v];
            vertices[i].normal = model.normals[idx.vn];
            vertices[i].texcoord = model.texcoords.empty() ? Vec2f() : model.texcoords[idx.vt];
        }

        // Transform vertices
        for (int i = 0; i < 3; ++i) {
            
            Vec4f pos(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z, 1.0f);
            // World to View
            pos = viewMatrix * pos; 
            // View to Clip
            pos = projectionMatrix * pos; 
            if (pos.w != 0.0f) {
                vertices[i].position = Vec3f(pos.x / pos.w, pos.y / pos.w, pos.z / pos.w);
            } else {
                std::cerr << "Warning: pos.w is 0.0f when transforming." << std::endl;;
            }
            // Normalize Device Coordinates (NDC)
            // Transform to Screen Space
            vertices[i].position.x = (vertices[i].position.x + 1.0f) * 0.5f * width;
            vertices[i].position.y = (vertices[i].position.y + 1.0f) * 0.5f * height;
            // std::cout << "vertices[" << i << "].position: " << vertices[i].position << std::endl;
            
        }
        // Rasterize triangle
        drawTriangle(vertices[0], vertices[1], vertices[2]);
    }
}

void Renderer::drawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
    // Bounding box for the triangle
    float minX = std::min({ v0.position.x, v1.position.x, v2.position.x });
    float minY = std::min({ v0.position.y, v1.position.y, v2.position.y });
    float maxX = std::max({ v0.position.x, v1.position.x, v2.position.x });
    float maxY = std::max({ v0.position.y, v1.position.y, v2.position.y });

    // Clamp to framebuffer
    int x0 = std::max(static_cast<int>(std::floor(minX)), 0);
    int y0 = std::max(static_cast<int>(std::floor(minY)), 0);
    int x1 = std::min(static_cast<int>(std::ceil(maxX)), width - 1);
    int y1 = std::min(static_cast<int>(std::ceil(maxY)), height - 1);

    // Precompute area

    float edge1x = v1.position.x - v0.position.x; 
    float edge1y = v1.position.y - v0.position.y;
    float edge2x = v2.position.x - v0.position.x;
    float edge2y = v2.position.y - v0.position.y;

    float denom = edge1x * edge2y - edge2x * edge1y;

    const float EPSILON = 1e-6f;
    if (std::abs(denom) < EPSILON)
        return; // Degenerate triangle


    Color RandColor(
        static_cast<uint8_t>(rand() % 255),
        static_cast<uint8_t>(rand() % 255),
        static_cast<uint8_t>(rand() % 255)
    );
    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            // Sample at pixel center
            float px = x + 0.5f;
            float py = y + 0.5f;

            float vx = x - v0.position.x;
            float vy = y - v0.position.y;

            // Compute barycentric coordinates

            float lambda1 = (edge2y * vx - edge2x * vy) / denom;
            float lambda2 = (-edge1y * vx + edge1x * vy) / denom;
            float lambda0 = 1.0f - lambda1 - lambda2;

            if (lambda0 < 0.0f || lambda1 < 0.0f || lambda2 < 0.0f)
                continue;
            
            float zP = lambda0 * v0.position.z + lambda1 * v1.position.z + lambda2 * v2.position.z;
            // float w2 = ((v0.position.x - v2.position.x) * (py - v2.position.y) -
            //             (v0.position.y - v2.position.y) * (px - v2.position.x)) / area;

            // If the point is inside the triangle
            
                // Perspective-correct interpolation



                // Interpolate normal
                Vec3f normal = (v0.normal * lambda0 + v1.normal * lambda1 + v2.normal * lambda2).normalized();

                // Interpolate position in view space for shading
                Vec3f fragPos = (v0.position * lambda0 + v1.position * lambda1 + v2.position * lambda2);

                // Shading
                Vec3f color = shader.fragment(fragPos, normal, Vec2f(), camera);

                // Convert color to 0-255
                // Color finalColor(
                //     static_cast<uint8_t>(std::min(color.x * 255.0f, 255.0f)),
                //     static_cast<uint8_t>(std::min(color.y * 255.0f, 255.0f)),
                //     static_cast<uint8_t>(std::min(color.z * 255.0f, 255.0f))
                // );

                Color depthColor(
                    static_cast<uint8_t>(std::min(zP * 150.f, 255.0f)),
                    static_cast<uint8_t>(std::min(zP * 150.f, 255.0f)),
                    static_cast<uint8_t>(std::min(zP * 150.f, 255.0f))
                );

                // Depth test and set pixel
                framebuffer.setPixel(x, y, RandColor, zP);
            
        }
    }
}
