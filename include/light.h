#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"

class Light {
public:
    Vec3f direction;
    Vec3f color;

    Light();
    Light(const Vec3f& dir, const Vec3f& col);
};

#endif // LIGHT_H
