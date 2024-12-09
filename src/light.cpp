#include "light.h"

Light::Light()
    : direction(0.0f, -1.0f, 0.0f),
      color(1.0f, 1.0f, 1.0f) {}

Light::Light(const Vec3f& dir, const Vec3f& col)
    : direction(dir.normalized()),
      color(col) {}
