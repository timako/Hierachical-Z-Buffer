// Mat.h
#ifndef Mat_H
#define Mat_H

#include <iostream>
#include <array>
#include <stdexcept>
#include "vector.h" 

// Forward declarations
class Vec3f;
class Vec4f;

// Mat3x3 Class
class Mat3x3 {
public:
    // Member variables: Row-major order
    float m[3][3];

    // Constructors
    Mat3x3(); // Default constructor (Identity Mat)
    Mat3x3(const std::array<std::array<float, 3>, 3>& elements); // Parameterized constructor
    Mat3x3(const Mat3x3& other); // Copy constructor

    // Destructor
    ~Mat3x3();

    // Operator Overloads
    Mat3x3 operator+(const Mat3x3& other) const;
    Mat3x3 operator-(const Mat3x3& other) const;
    Mat3x3 operator*(const Mat3x3& other) const; // Mat multiplication
    Mat3x3 operator*(float scalar) const; // Scalar multiplication
    Vec3f operator*(const Vec3f& vec) const; // Mat-vector multiplication

    Mat3x3& operator+=(const Mat3x3& other);
    Mat3x3& operator-=(const Mat3x3& other);
    Mat3x3& operator*=(const Mat3x3& other);
    Mat3x3& operator*=(float scalar);

    bool operator==(const Mat3x3& other) const;
    bool operator!=(const Mat3x3& other) const;

    // Utility Functions
    Mat3x3 transpose() const;
    float determinant() const;
    Mat3x3 inverse() const;

    // Friend Functions
    friend std::ostream& operator<<(std::ostream& os, const Mat3x3& mat);
};

// Scalar multiplication from the left
Mat3x3 operator*(float scalar, const Mat3x3& mat);

// Mat4x4 Class
class Mat4x4 {
public:
    // Member variables: Row-major order
    float m[4][4];

    // Constructors
    Mat4x4(); // Default constructor (Identity Mat)
    Mat4x4(const std::array<std::array<float, 4>, 4>& elements); // Parameterized constructor
    Mat4x4(const Mat4x4& other); // Copy constructor

    // Destructor
    ~Mat4x4();

    // Operator Overloads
    Mat4x4 operator+(const Mat4x4& other) const;
    Mat4x4 operator-(const Mat4x4& other) const;
    Mat4x4 operator*(const Mat4x4& other) const; // Mat multiplication
    Mat4x4 operator*(float scalar) const; // Scalar multiplication
    Vec4f operator*(const Vec4f& vec) const; // Mat-vector multiplication

    Mat4x4& operator+=(const Mat4x4& other);
    Mat4x4& operator-=(const Mat4x4& other);
    Mat4x4& operator*=(const Mat4x4& other);
    Mat4x4& operator*=(float scalar);

    bool operator==(const Mat4x4& other) const;
    bool operator!=(const Mat4x4& other) const;

    // Utility Functions
    Mat4x4 transpose() const;
    float determinant() const; // Optional: Implementing determinant for 4x4 is more involved
    Mat4x4 inverse() const; // Optional

    // Friend Functions
    friend std::ostream& operator<<(std::ostream& os, const Mat4x4& mat);
};

// Scalar multiplication from the left
Mat4x4 operator*(float scalar, const Mat4x4& mat);

#endif // Mat_H
