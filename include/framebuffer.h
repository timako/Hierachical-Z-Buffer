#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "vector.h"
#include <vector>
#include <cstdint>
#include "objtype.h"


class Renderer; 

class Framebuffer {
public:
    Renderer *pRenderer;
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
