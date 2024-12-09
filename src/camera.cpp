#include "camera.h"
#include <cmath>
#include <cstring>

// Utility functions for matrix operations
static void normalize(Vec3f& v) {
    float mag = v.magnitude();
    if (mag > 0.0f) {
        v /= mag;
    }
}

static Vec3f crossProduct(const Vec3f& a, const Vec3f& b) {
    return a.cross(b);
}

static float radians(float degrees) {
    return degrees * static_cast<float>(M_PI) / 180.0f;
}

Camera::Camera()
    : position(0.0f, 0.0f, 5.0f),
      target(0.0f, 0.0f, 0.0f),
      up(0.0f, 1.0f, 0.0f),
      fov(60.0f),
      aspectRatio(1.3333f), // 4:3
      nearPlane(0.1f),
      farPlane(100.0f) {}

Camera::Camera(const Vec3f& pos, const Vec3f& tgt, const Vec3f& upVec, float fieldOfView, float aspect, float nearP, float farP)
    : position(pos),
      target(tgt),
      up(upVec),
      fov(fieldOfView),
      aspectRatio(aspect),
      nearPlane(nearP),
      farPlane(farP) {}

// https://pbr-book.org/4ed/Geometry_and_Transformations/Transformations 
void Camera::getViewMatrix(Mat4x4 &matrix) const {
    Vec3f zaxis = (position - target).normalized();    // Forward
    Vec3f xaxis = crossProduct(up, zaxis).normalized(); // Right
    Vec3f yaxis = crossProduct(zaxis, xaxis);          // Up

    // Initialize to identity
    matrix.m[0][0] = xaxis.x;
    matrix.m[0][1] = xaxis.y;
    matrix.m[0][2] = xaxis.z;
    matrix.m[0][3] = -xaxis.dot(position);

    matrix.m[1][0] = yaxis.x;
    matrix.m[1][1] = yaxis.y;
    matrix.m[1][2] = yaxis.z;
    matrix.m[1][3] = -yaxis.dot(position);

    matrix.m[2][0] = zaxis.x;
    matrix.m[2][1] = zaxis.y;
    matrix.m[2][2] = zaxis.z;
    matrix.m[2][3] = -zaxis.dot(position);

    matrix.m[3][3] = 1.0f;
}

void Camera::getProjectionMatrix(Mat4x4 &matrix) const {
    float fovRad = radians(fov);
    float tanHalfFOV = tan(fovRad / 2.0f);
    float zRange = nearPlane - farPlane;

    matrix.m[0][0] = 1.0f / (tanHalfFOV * aspectRatio);
    matrix.m[1][1] = 1.0f / tanHalfFOV;
    matrix.m[2][2] = (-nearPlane - farPlane) / zRange;
    matrix.m[2][3] = 2.0f * farPlane * nearPlane / zRange;
    matrix.m[3][2] = 1.0f;
}
