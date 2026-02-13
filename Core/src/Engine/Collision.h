#pragma once

#include "Body.h"
#include <iostream>


namespace Core::Engine {


	//get verticies of OBB in world space
    inline void GetOBBVerts(const Body& b, std::vector<Vec2>& out) {

        Vec2 h = b.Shape.Half;

        // local corners (CCW)
        std::vector<Vec2> local = {
            { -h.x, -h.y },
            {  h.x, -h.y },
            {  h.x,  h.y },
            { -h.x,  h.y }
        };

        for (int i = 0; i < 4; ++i) {

			// rotate and translate to world space
            out[i] = b.Position + Rotate(local[i], b.Angle);
        }
    }

	// project verts onto one of 4 axis (in OBB case), return min/max scalar values of interval along that axis
    inline void ProjectOntoAxis(const std::vector<Vec2>& verts, const Vec2& axis, float& outMin, float& outMax) {
        float p0 = Dot(verts[0], axis);
        outMin = outMax = p0;
        for (int i = 1; i < 4; ++i) {
            float p = Dot(verts[i], axis);
            outMin = std::min(outMin, p);
            outMax = std::max(outMax, p);
        }
    }

    inline float IntervalOverlap(float minA, float maxA, float minB, float maxB) {
        // positive overlap => intersect
        return std::min(maxA, maxB) - std::max(minA, minB);
    }

    inline Vec2 SupportPoint(const Body& b, const Vec2& dir) {
		std::vector<Vec2> v(4);
        GetOBBVerts(b, v);
        float best = Dot(v[0], dir);
        Vec2 p = v[0];
        for (int i = 1; i < 4; ++i) {
            float d = Dot(v[i], dir);
            if (d > best) { best = d; p = v[i]; }
        }
        return p;
    }

    inline bool CollideOBBOBB(Body& A, Body& B, Manifold& m) {
        std::vector<Vec2> aV(4), bV(4);
        GetOBBVerts(A, aV);
        GetOBBVerts(B, bV);

        // SAT axes = normals of edges (2 from A, 2 from B)
        Vec2 axes[4] = {
            Normalize(aV[1] - aV[0]),
            Normalize(aV[3] - aV[0]),
            Normalize(bV[1] - bV[0]),
            Normalize(bV[3] - bV[0])
        };

        // Convert edge directions into outward normals (perp)
        for (int i = 0; i < 4; ++i) {
            axes[i] = Normalize(Vec2{ -axes[i].y, axes[i].x});
        }

        float bestOverlap = 1e30f;
        Vec2 bestAxis{ 0,0 };

        for (int i = 0; i < 4; ++i) {
            Vec2 axis = axes[i];

            float minA, maxA, minB, maxB;
            ProjectOntoAxis(aV, axis, minA, maxA);
            ProjectOntoAxis(bV, axis, minB, maxB);

            float overlap = IntervalOverlap(minA, maxA, minB, maxB);
            if (overlap <= 0.0f) return false; // separating axis found

			//select smallest overlap and its corresponding axis (for collision normal)
            if (overlap < bestOverlap) {
                bestOverlap = overlap;
                bestAxis = axis;
            }
        }

        // Ensure normal points from A -> B
        Vec2 d = B.Position - A.Position;
        if (Dot(d, bestAxis) < 0.0f) bestAxis *= -1.0f;

        m.Normal = bestAxis;
        m.Penetration = bestOverlap;

        // Simple stable-ish contact point approximation:
        // midpoint between support points along the collision normal
        Vec2 pA = SupportPoint(A, m.Normal * -1);
        Vec2 pB = SupportPoint(B, m.Normal);
        m.ContactPoint = (pA + pB) * 0.5f;

        return true;
    }

    inline bool CollideCircleOBB(Body& circle, Body& box, Manifold& m) {
        // Move circle center into box local space
        Vec2 rel = circle.Position - box.Position;
        Vec2 local = RotateInv(rel, box.Angle);

        Vec2 h = box.Shape.Half;

        // Closest point in local box (AABB clamp in local space)
        Vec2 closestLocal{
            std::clamp(local.x, -h.x, h.x),
            std::clamp(local.y, -h.y, h.y)
        };

        Vec2 dLocal = closestLocal - local; // from circle to box (in local)
        float dist = Len(dLocal);
        float r = circle.Shape.Radius;

        if (dist > r) return false;

        Vec2 nLocal;
        float penetration;

        if (dist > MIN_VEC_LEN) {
            nLocal = dLocal / dist;
            penetration = r - dist;
        }
        else {
            // Circle center inside box: choose closest face in local
            float left = std::fabs(local.x - (-h.x));
            float right = std::fabs(h.x - local.x);
            float bottom = std::fabs(local.y - (-h.y));
            float top = std::fabs(h.y - local.y);
            float minv = std::min({ left, right, bottom, top });

            if (minv == left)        nLocal = { -1,  0 }, penetration = r - left;
            else if (minv == right)  nLocal = { 1,  0 }, penetration = r - right;
            else if (minv == bottom) nLocal = { 0, -1 }, penetration = r - bottom;
            else                     nLocal = { 0,  1 }, penetration = r - top;

            // closest point is on that face
            closestLocal = { std::clamp(local.x, -h.x, h.x), std::clamp(local.y, -h.y, h.y) };
        }

        // Back to world space
        Vec2 normalWorld = Rotate(nLocal, box.Angle);
        Vec2 closestWorld = box.Position + Rotate(closestLocal, box.Angle);

        m.Normal = normalWorld;        // points circle(A) -> box(B)
        m.Penetration = penetration;
        m.ContactPoint = closestWorld;
        return true;
    }

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

        //AABB
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
        
        //OBB
        if (A.Shape.Type == ShapeType::OBB && B.Shape.Type == ShapeType::OBB)
            return CollideOBBOBB(A, B, m);

        if (A.Shape.Type == ShapeType::Circle && B.Shape.Type == ShapeType::OBB)
            return CollideCircleOBB(A, B, m);

        if (A.Shape.Type == ShapeType::OBB && B.Shape.Type == ShapeType::Circle) {
            bool hit = CollideCircleOBB(B, A, m);
            if (!hit) return false;
            m.Normal *= -1.0f; // keep normal A->B
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