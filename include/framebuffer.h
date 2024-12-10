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
    Framebuffer(int w, int h);
    void clear(const Color& clearColor = Color(0, 0, 0));
    void saveToBMP(const std::string& filename) const;
    virtual void setPixel(int x, int y, const Color& color, float depth) = 0; 
    virtual ~Framebuffer() = default;
    
};

class SimpleZbuffer : public Framebuffer{
public:
    std::vector<float> depthBuffer;
    SimpleZbuffer(int w, int h);
    void clear(const Color& clearColor = Color(0, 0, 0));
    virtual void setPixel(int x, int y, const Color& color, float depth);
};

#endif // FRAMEBUFFER_H
