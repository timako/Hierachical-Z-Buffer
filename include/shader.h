#ifndef SHADER_H
#define SHADER_H

#include "vector.h"
#include "light.h"
#include "camera.h"

struct Vertex {
    Vec3f position;
    Vec3f normal;
    Vec2f texcoord;
};

class Shader {
public:
    Light light;
    Vec3f ambientColor;
    Vec3f diffuseColor;
    Vec3f specularColor;
    float shininess;

    Shader();
    Shader(const Light& l, const Vec3f& ambient, const Vec3f& diffuse, const Vec3f& specular, float shin);

    // Fragment shader: computes color based on lighting
    Vec3f fragment(const Vec3f& fragPos, const Vec3f& normal, const Vec2f& texcoord, const Camera& camera) const;
};

#endif // SHADER_H
