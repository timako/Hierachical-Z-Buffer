#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "matrix.h"

class Camera {
public:
    Vec3f position;
    Vec3f target;
    Vec3f up;

    float fov; // Field of view in degrees
    float aspectRatio;
    float nearPlane;
    float farPlane;

    Camera();
    Camera(const Vec3f& pos, const Vec3f& tgt, const Vec3f& upVec, float fieldOfView, float aspect, float nearP, float farP);

    // Computes the View Matrix
    void getViewMatrix(Mat4x4 &ViewMat) const;

    // Computes the Projection Matrix
    void getProjectionMatrix(Mat4x4 &ProjMat) const;
};

#endif // CAMERA_H
