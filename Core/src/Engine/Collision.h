#pragma once

#include "Body.h"

namespace Core::Engine {

    inline Vec2 AABBMin(const Body& b) { return b.Position - b.Shape.Half; }
    inline Vec2 AABBMax(const Body& b) { return b.Position + b.Shape.Half; }

    inline Vec2 ClosestPointOnAABB(const Vec2& p, const Body& box) {
        Vec2 mn = AABBMin(box);
        Vec2 mx = AABBMax(box);
        return { std::clamp(p.x, mn.x, mx.x), std::clamp(p.y, mn.y, mx.y)};
    }

    inline bool CollideCircleCircle(Body& A, Body& B, Manifold& m) {
        Vec2 d = B.Position - A.Position;
        float r = A.Shape.Radius + B.Shape.Radius;
        float dist = Len(d);
        if (dist >= r) return false;

        Vec2 n = (dist > MIN_VEC_LEN) ? (d / dist) : Vec2{ 1, 0 };

        m.Normal = n;
        m.Penetration = r - dist;
        m.ContactPoint = A.Position + n * A.Shape.Radius;
        return true;
    }

    inline bool CollideAABBAABB(Body& A, Body& B, Manifold& m) {
        Vec2 d = B.Position - A.Position;
        Vec2 a = A.Shape.Half;
        Vec2 b = B.Shape.Half;

        float ox = (a.x + b.x) - std::fabs(d.x);
        if (ox <= 0) return false;

        float oy = (a.y + b.y) - std::fabs(d.y);
        if (oy <= 0) return false;

        // choose axis of least penetration
        if (ox < oy) {
            m.Penetration = ox;
            m.Normal = (d.x < 0) ? Vec2{ -1, 0 } : Vec2{ 1, 0 };
        }
        else {
            m.Penetration = oy;
            m.Normal = (d.y < 0) ? Vec2{ 0, -1 } : Vec2{ 0, 1 };
        }

        // simple contact point approximation: midpoint of overlap along Normal
        m.ContactPoint = (A.Position + B.Position) * 0.5f;
        return true;
    }

    inline bool CollideCircleAABB(Body& circle, Body& box, Manifold& m) {
        Vec2 closest = ClosestPointOnAABB(circle.Position, box);
        Vec2 d = closest - circle.Position;

        float dist = Len(d);
        float r = circle.Shape.Radius;
        if (dist > r) return false;


        // If circle center is inside box, d is ~0; pick a Normal from box faces
        Vec2 n;
        if (dist > MIN_VEC_LEN) {
            n = Normalize(d); // from circle to closest point (toward box)
            m.Penetration = r - dist;
        }
        else {
            // inside: choose direction of minimum distance to a face
            Vec2 mn = AABBMin(box), mx = AABBMax(box);
            float left = std::fabs(circle.Position.x - mn.x);
            float right = std::fabs(mx.x - circle.Position.x);
            float bottom = std::fabs(circle.Position.y - mn.y);
            float top = std::fabs(mx.y - circle.Position.y);
            float minv = std::min({ left, right, bottom, top });
            if (minv == left)   n = { -1, 0 };
            else if (minv == right)  n = { 1, 0 };
            else if (minv == bottom) n = { 0, -1 };
            else n = { 0, 1 };
            m.Penetration = r - minv;
        }

        m.Normal = n;
        m.ContactPoint = closest;
        return true;
    }

    inline bool BuildManifold(Body& A, Body& B, Manifold& m) {
        if (A.Shape.Type == ShapeType::Circle && B.Shape.Type == ShapeType::Circle)
            return CollideCircleCircle(A, B, m);

        if (A.Shape.Type == ShapeType::AABB && B.Shape.Type == ShapeType::AABB)
            return CollideAABBAABB(A, B, m);

        if (A.Shape.Type == ShapeType::Circle && B.Shape.Type == ShapeType::AABB)
            return CollideCircleAABB(A, B, m);

        if (A.Shape.Type == ShapeType::AABB && B.Shape.Type == ShapeType::Circle) {
            // swap, then flip Normal so it remains A->B
            bool hit = CollideCircleAABB(B, A, m);
            if (!hit) return false;
            m.Normal *= -1.0f;
            return true;
        }

        return false;
    }

    inline void PositionalCorrection(Body& A, Body& B, const Manifold& m) {
        const float percent = 0.2f; // 20%
        const float slop = 0.01f;

        float invMassSum = A.invMass + B.invMass;
        if (invMassSum <= 0) return;

        float corrMag = std::max(m.Penetration - slop, 0.0f) / invMassSum;
        Vec2 correction = m.Normal * (corrMag * percent);

        A.Position -= correction * A.invMass;
        B.Position += correction * B.invMass;
    }


}