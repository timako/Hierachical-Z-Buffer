// Mat.cpp
#include "matrix.h"
#include <cmath>
#include <iomanip>

// -------------------- Mat3x3 Implementation -------------------- //

// Default constructor (Identity Mat)
Mat3x3::Mat3x3() {
    // Initialize to identity Mat
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] = (i == j) ? 1.0f : 0.0f;
}

// Parameterized constructor
Mat3x3::Mat3x3(const std::array<std::array<float, 3>, 3>& elements) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] = elements[i][j];
}

// Copy constructor
Mat3x3::Mat3x3(const Mat3x3& other) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] = other.m[i][j];
}

// Destructor
Mat3x3::~Mat3x3() {}

// Operator Overloads

Mat3x3 Mat3x3::operator+(const Mat3x3& other) const {
    Mat3x3 result;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            result.m[i][j] = this->m[i][j] + other.m[i][j];
    return result;
}

Mat3x3 Mat3x3::operator-(const Mat3x3& other) const {
    Mat3x3 result;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            result.m[i][j] = this->m[i][j] - other.m[i][j];
    return result;
}

Mat3x3 Mat3x3::operator*(const Mat3x3& other) const {
    Mat3x3 result;
    for(int i=0;i<3;i++) {
        for(int j=0;j<3;j++) {
            result.m[i][j] = 0.0f;
            for(int k=0;k<3;k++) {
                result.m[i][j] += this->m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

Mat3x3 Mat3x3::operator*(float scalar) const {
    Mat3x3 result;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            result.m[i][j] = this->m[i][j] * scalar;
    return result;
}

Vec3f Mat3x3::operator*(const Vec3f& vec) const {
    Vec3f result;
    result.x = m[0][0]*vec.x + m[0][1]*vec.y + m[0][2]*vec.z;
    result.y = m[1][0]*vec.x + m[1][1]*vec.y + m[1][2]*vec.z;
    result.z = m[2][0]*vec.x + m[2][1]*vec.y + m[2][2]*vec.z;
    return result;
}

Mat3x3& Mat3x3::operator+=(const Mat3x3& other) {
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            this->m[i][j] += other.m[i][j];
    return *this;
}

Mat3x3& Mat3x3::operator-=(const Mat3x3& other) {
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            this->m[i][j] -= other.m[i][j];
    return *this;
}

Mat3x3& Mat3x3::operator*=(const Mat3x3& other) {
    *this = *this * other;
    return *this;
}

Mat3x3& Mat3x3::operator*=(float scalar) {
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            this->m[i][j] *= scalar;
    return *this;
}

bool Mat3x3::operator==(const Mat3x3& other) const {
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            if(this->m[i][j] != other.m[i][j])
                return false;
    return true;
}

bool Mat3x3::operator!=(const Mat3x3& other) const {
    return !(*this == other);
}

// Utility Functions

Mat3x3 Mat3x3::transpose() const {
    Mat3x3 transposed;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            transposed.m[i][j] = this->m[j][i];
    return transposed;
}

float Mat3x3::determinant() const {
    // Using Sarrus' rule
    float det = 0.0f;
    det += m[0][0]*(m[1][1]*m[2][2] - m[1][2]*m[2][1]);
    det -= m[0][1]*(m[1][0]*m[2][2] - m[1][2]*m[2][0]);
    det += m[0][2]*(m[1][0]*m[2][1] - m[1][1]*m[2][0]);
    return det;
}

Mat3x3 Mat3x3::inverse() const {
    float det = determinant();
    if(det == 0.0f)
        throw std::runtime_error("Mat3x3 is singular and cannot be inverted.");

    Mat3x3 inv;
    inv.m[0][0] = (m[1][1]*m[2][2] - m[1][2]*m[2][1]) / det;
    inv.m[0][1] = (m[0][2]*m[2][1] - m[0][1]*m[2][2]) / det;
    inv.m[0][2] = (m[0][1]*m[1][2] - m[0][2]*m[1][1]) / det;

    inv.m[1][0] = (m[1][2]*m[2][0] - m[1][0]*m[2][2]) / det;
    inv.m[1][1] = (m[0][0]*m[2][2] - m[0][2]*m[2][0]) / det;
    inv.m[1][2] = (m[0][2]*m[1][0] - m[0][0]*m[1][2]) / det;

    inv.m[2][0] = (m[1][0]*m[2][1] - m[1][1]*m[2][0]) / det;
    inv.m[2][1] = (m[0][1]*m[2][0] - m[0][0]*m[2][1]) / det;
    inv.m[2][2] = (m[0][0]*m[1][1] - m[0][1]*m[1][0]) / det;

    return inv;
}

// Friend Functions

std::ostream& operator<<(std::ostream& os, const Mat3x3& mat) {
    os << std::fixed << std::setprecision(2);
    for(int i=0;i<3;i++) {
        os << "[ ";
        for(int j=0;j<3;j++) {
            os << mat.m[i][j] << " ";
        }
        os << "]\n";
    }
    return os;
}

// Scalar multiplication from the left
Mat3x3 operator*(float scalar, const Mat3x3& mat) {
    return mat * scalar;
}

// -------------------- Mat4x4 Implementation -------------------- //

// Default constructor (Identity Mat)
Mat4x4::Mat4x4() {
    // Initialize to identity Mat
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] =  0.0f;
}

// Parameterized constructor
Mat4x4::Mat4x4(const std::array<std::array<float, 4>, 4>& elements) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] = elements[i][j];
}

// Copy constructor
Mat4x4::Mat4x4(const Mat4x4& other) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] = other.m[i][j];
}

// Destructor
Mat4x4::~Mat4x4() {}

// Operator Overloads

Mat4x4 Mat4x4::operator+(const Mat4x4& other) const {
    Mat4x4 result;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            result.m[i][j] = this->m[i][j] + other.m[i][j];
    return result;
}

Mat4x4 Mat4x4::operator-(const Mat4x4& other) const {
    Mat4x4 result;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            result.m[i][j] = this->m[i][j] - other.m[i][j];
    return result;
}

Mat4x4 Mat4x4::operator*(const Mat4x4& other) const {
    Mat4x4 result;
    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            result.m[i][j] = 0.0f;
            for(int k=0;k<4;k++) {
                result.m[i][j] += this->m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

Mat4x4 Mat4x4::operator*(float scalar) const {
    Mat4x4 result;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            result.m[i][j] = this->m[i][j] * scalar;
    return result;
}

Vec4f Mat4x4::operator*(const Vec4f& vec) const {
    Vec4f result;
    result.x = m[0][0]*vec.x + m[0][1]*vec.y + m[0][2]*vec.z + m[0][3]*vec.w;
    result.y = m[1][0]*vec.x + m[1][1]*vec.y + m[1][2]*vec.z + m[1][3]*vec.w;
    result.z = m[2][0]*vec.x + m[2][1]*vec.y + m[2][2]*vec.z + m[2][3]*vec.w;
    result.w = m[3][0]*vec.x + m[3][1]*vec.y + m[3][2]*vec.z + m[3][3]*vec.w;
    return result;
}

Mat4x4& Mat4x4::operator+=(const Mat4x4& other) {
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            this->m[i][j] += other.m[i][j];
    return *this;
}

Mat4x4& Mat4x4::operator-=(const Mat4x4& other) {
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            this->m[i][j] -= other.m[i][j];
    return *this;
}

Mat4x4& Mat4x4::operator*=(const Mat4x4& other) {
    *this = *this * other;
    return *this;
}

Mat4x4& Mat4x4::operator*=(float scalar) {
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            this->m[i][j] *= scalar;
    return *this;
}

bool Mat4x4::operator==(const Mat4x4& other) const {
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(this->m[i][j] != other.m[i][j])
                return false;
    return true;
}

bool Mat4x4::operator!=(const Mat4x4& other) const {
    return !(*this == other);
}

// Utility Functions

Mat4x4 Mat4x4::transpose() const {
    Mat4x4 transposed;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            transposed.m[i][j] = this->m[j][i];
    return transposed;
}

// Determinant and inverse for 4x4 matrices are more complex.
// Implementing them is beyond the scope of this example.
// You can use libraries like Eigen or GLM for advanced Mat operations.

// Friend Functions

std::ostream& operator<<(std::ostream& os, const Mat4x4& mat) {
    os << std::fixed << std::setprecision(2);
    for(int i=0;i<4;i++) {
        os << "[ ";
        for(int j=0;j<4;j++) {
            os << mat.m[i][j] << " ";
        }
        os << "]\n";
    }
    return os;
}

// Scalar multiplication from the left
Mat4x4 operator*(float scalar, const Mat4x4& mat) {
    return mat * scalar;
}

