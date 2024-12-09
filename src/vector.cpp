#include "vector.h"

// Default constructor initializes to zero vector
Vec3f::Vec3f() : x(0.0f), y(0.0f), z(0.0f) {}

// Parameterized constructor
Vec3f::Vec3f(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

// Copy constructor
Vec3f::Vec3f(const Vec3f& other) : x(other.x), y(other.y), z(other.z) {}

// Destructor
Vec3f::~Vec3f() {}

// Operator Overloads

// Addition
Vec3f Vec3f::operator+(const Vec3f& other) const {
    return Vec3f(x + other.x, y + other.y, z + other.z);
}

// Subtraction
Vec3f Vec3f::operator-(const Vec3f& other) const {
    return Vec3f(x - other.x, y - other.y, z - other.z);
}

// Scalar multiplication
Vec3f Vec3f::operator*(float scalar) const {
    return Vec3f(x * scalar, y * scalar, z * scalar);
}

// Scalar division
Vec3f Vec3f::operator/(float scalar) const {
    if (scalar == 0.0f) {
        throw std::runtime_error("Division by zero");
    }
    return Vec3f(x / scalar, y / scalar, z / scalar);
}

// Compound addition
Vec3f& Vec3f::operator+=(const Vec3f& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

// Compound subtraction
Vec3f& Vec3f::operator-=(const Vec3f& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

// Compound scalar multiplication
Vec3f& Vec3f::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

// Compound scalar division
Vec3f& Vec3f::operator/=(float scalar) {
    if (scalar == 0.0f) {
        throw std::runtime_error("Division by zero");
    }
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

// Equality
bool Vec3f::operator==(const Vec3f& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

// Inequality
bool Vec3f::operator!=(const Vec3f& other) const {
    return !(*this == other);
}

// Dot product
float Vec3f::dot(const Vec3f& other) const {
    return x * other.x + y * other.y + z * other.z;
}

// Cross product
Vec3f Vec3f::cross(const Vec3f& other) const {
    return Vec3f(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

// Magnitude (length) of the vector
float Vec3f::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

// Normalized vector (unit vector)
Vec3f Vec3f::normalized() const {
    float mag = magnitude();
    if (mag == 0.0f) {
        throw std::runtime_error("Cannot normalize zero vector");
    }
    return (*this) / mag;
}

// Normalize the vector in place
void Vec3f::normalize() {
    float mag = magnitude();
    if (mag == 0.0f) {
        throw std::runtime_error("Cannot normalize zero vector");
    }
    (*this) /= mag;
}

// Output stream overload for easy printing
std::ostream& operator<<(std::ostream& os, const Vec3f& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

// Scalar multiplication from the left
Vec3f operator*(float scalar, const Vec3f& vec) {
    return vec * scalar;
}

// Default constructor initializes to zero vector
Vec2f::Vec2f() : u(0.0f), v(0.0f) {}

// Parameterized constructor
Vec2f::Vec2f(float u_, float v_) : u(u_), v(v_) {}

// Copy constructor
Vec2f::Vec2f(const Vec2f& other) : u(other.u), v(other.v) {}

// Destructor
Vec2f::~Vec2f() {}

// Operator Overloads

Vec2f Vec2f::operator+(const Vec2f& other) const {
    return Vec2f(u + other.u, v + other.v);
}

Vec2f Vec2f::operator-(const Vec2f& other) const {
    return Vec2f(u - other.u, v - other.v);
}

Vec2f Vec2f::operator*(float scalar) const {
    return Vec2f(u * scalar, v * scalar);
}

Vec2f Vec2f::operator/(float scalar) const {
    if (scalar == 0.0f) {
        throw std::runtime_error("Division by zero");
    }
    return Vec2f(u / scalar, v / scalar);
}

Vec2f& Vec2f::operator+=(const Vec2f& other) {
    u += other.u;
    v += other.v;
    return *this;
}

Vec2f& Vec2f::operator-=(const Vec2f& other) {
    u -= other.u;
    v -= other.v;
    return *this;
}

Vec2f& Vec2f::operator*=(float scalar) {
    u *= scalar;
    v *= scalar;
    return *this;
}

Vec2f& Vec2f::operator/=(float scalar) {
    if (scalar == 0.0f) {
        throw std::runtime_error("Division by zero");
    }
    u /= scalar;
    v /= scalar;
    return *this;
}

Vec3f Vec3f::operator-() const {
    return Vec3f(-x, -y, -z);
}

bool Vec2f::operator==(const Vec2f& other) const {
    return (u == other.u && v == other.v);
}

bool Vec2f::operator!=(const Vec2f& other) const {
    return !(*this == other);
}

// Vector Operations

float Vec2f::dot(const Vec2f& other) const {
    return u * other.u + v * other.v;
}

float Vec2f::magnitude() const {
    return std::sqrt(u * u + v * v);
}

Vec2f Vec2f::normalized() const {
    float mag = magnitude();
    if (mag == 0.0f) {
        throw std::runtime_error("Cannot normalize zero vector");
    }
    return (*this) / mag;
}

void Vec2f::normalize() {
    float mag = magnitude();
    if (mag == 0.0f) {
        throw std::runtime_error("Cannot normalize zero vector");
    }
    (*this) /= mag;
}

// Output stream overload for easy printing
std::ostream& operator<<(std::ostream& os, const Vec2f& vec) {
    os << "(" << vec.u << ", " << vec.v << ")";
    return os;
}

// Scalar multiplication from the left
Vec2f operator*(float scalar, const Vec2f& vec) {
    return vec * scalar;
}

// Default constructor
Vec4f::Vec4f() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

// Parameterized constructor
Vec4f::Vec4f(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}

// Copy constructor
Vec4f::Vec4f(const Vec4f& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

// Destructor
Vec4f::~Vec4f() {}

// Operator Overloads

Vec4f Vec4f::operator+(const Vec4f& other) const {
    return Vec4f(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vec4f Vec4f::operator-(const Vec4f& other) const {
    return Vec4f(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vec4f Vec4f::operator*(float scalar) const {
    return Vec4f(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vec4f Vec4f::operator/(float scalar) const {
    if (scalar == 0.0f) {
        throw std::runtime_error("Division by zero");
    }
    return Vec4f(x / scalar, y / scalar, z / scalar, w / scalar);
}

Vec4f& Vec4f::operator+=(const Vec4f& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

Vec4f& Vec4f::operator-=(const Vec4f& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

Vec4f& Vec4f::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

Vec4f& Vec4f::operator/=(float scalar) {
    if (scalar == 0.0f) {
        throw std::runtime_error("Division by zero");
    }
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
}

Vec4f Vec4f::operator-() const {
    return Vec4f(-x, -y, -z, -w);
}

bool Vec4f::operator==(const Vec4f& other) const {
    return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
}

bool Vec4f::operator!=(const Vec4f& other) const {
    return !(*this == other);
}

// Vector Operations

float Vec4f::dot(const Vec4f& other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

float Vec4f::magnitude() const {
    return std::sqrt(this->dot(*this));
}

Vec4f Vec4f::normalized() const {
    float mag = magnitude();
    if (mag == 0.0f) {
        throw std::runtime_error("Cannot normalize a zero vector");
    }
    return *this / mag;
}

void Vec4f::normalize() {
    float mag = magnitude();
    if (mag == 0.0f) {
        throw std::runtime_error("Cannot normalize a zero vector");
    }
    x /= mag;
    y /= mag;
    z /= mag;
    w /= mag;
}

// Friend Functions

std::ostream& operator<<(std::ostream& os, const Vec4f& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    return os;
}

// Scalar multiplication from the left
Vec4f operator*(float scalar, const Vec4f& vec) {
    return vec * scalar;
}