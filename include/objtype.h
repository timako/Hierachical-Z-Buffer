#ifndef OBJTYPE_H
#define OBJTYPE_H
#include "vector.h"

struct Vertex {
    Vec3f position;
    Vec3f normal;
    Vec2f texcoord;
};

struct Color {
    uint8_t r, g, b;

    Color() : r(0), g(0), b(0) {}
    Color(uint8_t red, uint8_t green, uint8_t blue)
        : r(red), g(green), b(blue) {}
};

#endif // OBJTYPE_H