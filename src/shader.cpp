#include "shader.h"
#include <cmath>

Shader::Shader()
    : light(),
      ambientColor(0.1f, 0.1f, 0.1f),
      diffuseColor(0.5f, 0.5f, 0.5f),
      specularColor(1.0f, 1.0f, 1.0f),
      shininess(32.0f) {}

Shader::Shader(const Light& l, const Vec3f& ambient, const Vec3f& diffuse, const Vec3f& specular, float shin)
    : light(l),
      ambientColor(ambient),
      diffuseColor(diffuse),
      specularColor(specular),
      shininess(shin) {}

Vec3f Shader::fragment(const Vec3f& fragPos, const Vec3f& normal, const Vec2f& texcoord, const Camera& camera) const {
    // Ambient
    Vec3f ambient = ambientColor;

    // Diffuse
    Vec3f lightDir = (-light.direction).normalized();
    float diff = std::max(normal.dot(lightDir), 0.0f);
    Vec3f diffuse = diffuseColor * diff;

    // Specular
    Vec3f viewDir = (camera.position - fragPos).normalized();
    Vec3f reflectDir = (2.0f * normal.dot(lightDir) * normal - lightDir).normalized();
    float spec = std::pow(std::max(viewDir.dot(reflectDir), 0.0f), shininess);
    Vec3f specular = specularColor * spec;

    // Combine results
    Vec3f result = ambient + diffuse + specular;
    // Clamp the result
    result.x = std::min(result.x, 1.0f);
    result.y = std::min(result.y, 1.0f);
    result.z = std::min(result.z, 1.0f);

    // return random color to debug fragment
    // return Vec3f(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f);
    return result;
}
