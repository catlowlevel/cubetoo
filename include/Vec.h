#pragma once
#include <cmath>
#include <format>
#include <string>

struct Vec3
{
    float x, y, z;

    Vec3() : x(0), y(0), z(0)
    {
    }

    Vec3(float x, float y, float z) : x(x), y(y), z(z)
    {
    }

    Vec3 operator+(const Vec3& other) const noexcept
    {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator-(const Vec3& other) const noexcept
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator*(const float& scalar) const noexcept
    {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    Vec3 operator/(const float& scalar) const
    {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }

    // bool operator==(const Vec3& other) const noexcept
    // {
    //     return x == other.x && y == other.y && z == other.z;
    // }

    // bool operator!=(const Vec3& other) const noexcept
    // {
    //     return !(x == other.x && y == other.y && z == other.z);
    // }

    float length() const noexcept
    {
        return sqrt(x * x + y * y + z * z);
    }

    void normalize()
    {
        float len = this->length();
        x /= len;
        y /= len;
        z /= len;
    }

    // Trigonometry functions
    float dot(const Vec3& other) const noexcept
    {
        return x * other.x + y * other.y + z * other.z;
    }

    float cross(const Vec3& other) const noexcept
    {
        return x * other.y - y * other.x + z * other.z;
    }

    float angle(const Vec3& other) const noexcept
    {
        return acos(this->dot(other) / (this->length() * other.length()));
    }

    float dist(const Vec3& other) const noexcept
    {
        float dx = other.x - x;
        float dy = other.y - y;
        float dz = other.z - z;
        return sqrtf(dx * dx + dy * dy + dz * dz);
    }

    operator std::string() const noexcept
    {
        return std::format("{}, {}, {}", x, y, z);
    }
};

struct Vec2
{
    float x{};
    float y{};

    Vec2()
    {
    }
    Vec2(float x, float y) : x(x), y(y)
    {
    }

    // Arithmetic operators
    Vec2 operator+(const Vec2& v) const noexcept
    {
        return Vec2(x + v.x, y + v.y);
    }
    Vec2 operator-(const Vec2& v) const noexcept
    {
        return Vec2(x - v.x, y - v.y);
    }
    Vec2 operator*(const Vec2& v) const noexcept
    {
        return Vec2(x * v.x - y * v.y, x * v.y + y * v.x);
    }
    Vec2 operator*(float s) const noexcept
    {
        return Vec2(x * s, y * s);
    }
    Vec2 operator/(float s) const
    {
        return Vec2(x / s, y / s);
    }

    // Comparison operators
    bool operator==(const Vec2& v) const noexcept
    {
        return x == v.x && y == v.y;
    }
    bool operator!=(const Vec2& v) const noexcept
    {
        return !(*this == v);
    }

    // Normalization
    void normalize()
    {
        float len = this->length();
        x /= len;
        y /= len;
    }

    // Trigonometry
    float angle() const noexcept
    {
        return atan2(y, x);
    }
    float length() const noexcept
    {
        return sqrt(x * x + y * y);
    }

    float dist(const Vec2& other) const noexcept
    {
        float dx = other.x - x;
        float dy = other.y - y;
        return sqrtf(dx * dx + dy * dy);
    }

    operator std::string() const noexcept
    {
        return std::format("{}, {}", x, y);
    }
};

struct Vec4
{
    float x{}, y{}, z{}, w{};

    Vec4 operator*(const float scalar) const
    {
        Vec4 result;
        result.x = x * scalar;
        result.y = y * scalar;
        result.z = z * scalar;
        result.w = w * scalar;
        return result;
    }

    Vec4 operator+(const Vec4& other) const
    {
        Vec4 result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;
        result.w = w + other.w;
        return result;
    }

    Vec4 operator-(const Vec4& other) const
    {
        Vec4 result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;
        result.w = w - other.w;
        return result;
    }
};