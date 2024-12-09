#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "vector.h"
#include <vector>
#include <cstdint>

struct Color {
    uint8_t r, g, b;

    Color() : r(0), g(0), b(0) {}
    Color(uint8_t red, uint8_t green, uint8_t blue)
        : r(red), g(green), b(blue) {}
};

class Framebuffer {
public:
    int width;
    int height;
    std::vector<Color> colorBuffer;
    std::vector<float> depthBuffer;

    Framebuffer(int w, int h);

    void clear(const Color& clearColor = Color(0, 0, 0));
    void setPixel(int x, int y, const Color& color, float depth);
    void saveToBMP(const std::string& filename) const;
};

#endif // FRAMEBUFFER_H
