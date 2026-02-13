#pragma once

#include <cmath>
#include <algorithm>

#define MIN_VEC_LEN 1e-8f

namespace Core::Engine {

    struct Vec2 {
        float x = 0, y = 0;

        Vec2() = default;
        Vec2(float x_, float y_) : x(x_), y(y_) {}

        Vec2 operator + (const Vec2& o) const { return { x + o.x, y + o.y }; }
        Vec2 operator - (const Vec2& o) const { return { x - o.x, y - o.y }; }
        Vec2 operator * (float s) const { return { x * s, y * s }; }
        Vec2 operator / (float s) const { return { x / s, y / s }; }

        Vec2& operator += (const Vec2& o) { x += o.x; y += o.y; return *this; }
        Vec2& operator -= (const Vec2& o) { x -= o.x; y -= o.y; return *this; }
        Vec2& operator *= (float s) { x *= s; y *= s; return *this; }
    };

    inline float Dot(const Vec2& a, const Vec2& b) { return a.x * b.x + a.y * b.y; }
    inline float Len(const Vec2& v) { return std::sqrt(Dot(v, v)); }

    inline Vec2 Normalize(const Vec2& v) {
        float l = Len(v);
        if (l <= MIN_VEC_LEN) return { 0, 0 };
        return v / l;
    }

    inline Vec2 Cross(float w, const Vec2& v) {
        return Vec2(-w * v.y, w * v.x);
    }

    inline float Cross(const Vec2& a, const Vec2& b) {
        return a.x * b.y - a.y * b.x;
    }

    inline Vec2 Rotate(const Vec2& v, float a) {
        float c = std::cos(a), s = std::sin(a);
        return { v.x * c - v.y * s, v.x * s + v.y * c };
    }

    inline Vec2 RotateInv(const Vec2& v, float a) {
        // rotate by -a
        float c = std::cos(a), s = std::sin(a);
        return { v.x * c + v.y * s, -v.x * s + v.y * c };
    }

}