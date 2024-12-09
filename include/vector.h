#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>

class Vec3f {
public:
    // Member variables
    float x, y, z;

    // Constructors
    Vec3f(); // Default constructor
    Vec3f(float x, float y, float z); // Parameterized constructor

    // Copy constructor
    Vec3f(const Vec3f& other);

    // Destructor
    ~Vec3f();

    // Operator Overloads
    Vec3f operator+(const Vec3f& other) const;
    Vec3f operator-(const Vec3f& other) const;
    Vec3f operator*(float scalar) const;
    Vec3f operator/(float scalar) const;

    Vec3f& operator+=(const Vec3f& other);
    Vec3f& operator-=(const Vec3f& other);
    Vec3f& operator*=(float scalar);
    Vec3f& operator/=(float scalar);

    Vec3f operator-() const;


    bool operator==(const Vec3f& other) const;
    bool operator!=(const Vec3f& other) const;

    // Vector Operations
    float dot(const Vec3f& other) const;
    Vec3f cross(const Vec3f& other) const;
    float magnitude() const;
    Vec3f normalized() const;
    void normalize();

    // Friend Functions
    friend std::ostream& operator<<(std::ostream& os, const Vec3f& vec);
};

// Scalar multiplication from the left
Vec3f operator*(float scalar, const Vec3f& vec);

class Vec2f {
public:
    float u, v;

    // Constructors
    Vec2f(); // Default constructor
    Vec2f(float u, float v); // Parameterized constructor

    // Copy constructor
    Vec2f(const Vec2f& other);

    // Destructor
    ~Vec2f();

    // Operator Overloads
    Vec2f operator+(const Vec2f& other) const;
    Vec2f operator-(const Vec2f& other) const;
    Vec2f operator*(float scalar) const;
    Vec2f operator/(float scalar) const;

    Vec2f& operator+=(const Vec2f& other);
    Vec2f& operator-=(const Vec2f& other);
    Vec2f& operator*=(float scalar);
    Vec2f& operator/=(float scalar);

    bool operator==(const Vec2f& other) const;
    bool operator!=(const Vec2f& other) const;
    

    // Vector Operations
    float dot(const Vec2f& other) const;
    float magnitude() const;
    Vec2f normalized() const;
    void normalize();

    // Friend Functions
    friend std::ostream& operator<<(std::ostream& os, const Vec2f& vec);
};

// Scalar multiplication from the left
Vec2f operator*(float scalar, const Vec2f& vec);

class Vec4f {
public:
    // Member variables
    float x, y, z, w;

    // Constructors
    Vec4f(); // Default constructor
    Vec4f(float x, float y, float z, float w); // Parameterized constructor

    // Copy constructor
    Vec4f(const Vec4f& other);

    // Destructor
    ~Vec4f();

    // Operator Overloads
    Vec4f operator+(const Vec4f& other) const;
    Vec4f operator-(const Vec4f& other) const;
    Vec4f operator*(float scalar) const;
    Vec4f operator/(float scalar) const;

    Vec4f& operator+=(const Vec4f& other);
    Vec4f& operator-=(const Vec4f& other);
    Vec4f& operator*=(float scalar);
    Vec4f& operator/=(float scalar);

    Vec4f operator-() const;

    bool operator==(const Vec4f& other) const;
    bool operator!=(const Vec4f& other) const;

    // Vector Operations
    float dot(const Vec4f& other) const;
    // Note: Cross product is not standard for 4D vectors. Implement if needed.
    float magnitude() const;
    Vec4f normalized() const;
    void normalize();

    // Friend Functions
    friend std::ostream& operator<<(std::ostream& os, const Vec4f& vec);
};

// Scalar multiplication from the left
Vec4f operator*(float scalar, const Vec4f& vec);

#endif // VECTOR_H