#pragma once

#include "Body.h"

namespace Core::Engine {

    inline int ClipSegmentToLine(Vec2 out[2], Vec2 in[2], const Vec2& n, float c) {
        // Keep points where Dot(n, x) <= c
        int sp = 0;

        float d0 = Dot(n, in[0]) - c;
        float d1 = Dot(n, in[1]) - c;

        if (d0 <= 0.0f) out[sp++] = in[0];
        if (d1 <= 0.0f) out[sp++] = in[1];

        if (d0 * d1 < 0.0f) {
            float t = d0 / (d0 - d1);
            out[sp++] = in[0] + (in[1] - in[0]) * t;
        }

        return sp;
    }

    // verts order from GetOBBVerts: 0(-x,-y), 1(x,-y), 2(x,y), 3(-x,y)
    inline void GetFaceVertices(const std::vector<Vec2>& v, const Vec2& faceNormal, Vec2& outV1, Vec2& outV2) {
        // Determine which faceNormal aligns with, using dot to pick +/-X or +/-Y in world space.
        // We'll compare against the box's world axes derived from its verts:
        Vec2 edgeX = Normalize(v[1] - v[0]); // roughly +X axis (rotated)
        Vec2 edgeY = Normalize(v[3] - v[0]); // roughly +Y axis (rotated)

        float dx = Dot(faceNormal, edgeX);
        float dy = Dot(faceNormal, edgeY);

        if (std::fabs(dx) > std::fabs(dy)) {
            if (dx > 0) { outV1 = v[1]; outV2 = v[2]; } // +X face
            else { outV1 = v[3]; outV2 = v[0]; } // -X face
        }
        else {
            if (dy > 0) { outV1 = v[2]; outV2 = v[3]; } // +Y face
            else { outV1 = v[0]; outV2 = v[1]; } // -Y face
        }
    }

    inline void GetIncidentFace(const std::vector<Vec2>& vInc, const Vec2& refNormal, Vec2 out[2]) {
        // pick incident face whose normal is most anti-parallel to refNormal
        Vec2 edgeX = Normalize(vInc[1] - vInc[0]);
        Vec2 edgeY = Normalize(vInc[3] - vInc[0]);

        Vec2 normals[4] = { edgeX, edgeX * -1, edgeY, edgeY * -1}; // approx face normals
        float minDot = 1e30f;
        Vec2 bestN{ 0,0 };

        for (int i = 0; i < 4; ++i) {
            float d = Dot(refNormal, normals[i]);
            if (d < minDot) { minDot = d; bestN = normals[i]; }
        }

        Vec2 v1, v2;
        GetFaceVertices(vInc, bestN, v1, v2);
        out[0] = v1;
        out[1] = v2;
    }
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
    inline void ProjectPointOntoAxis(const std::vector<Vec2>& verts, const Vec2& axis, float& outMin, float& outMax) {
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
        int bestAxisIndex = -1;

        for (int i = 0; i < 4; ++i) {
            Vec2 axis = axes[i];

            float minA, maxA, minB, maxB;
            ProjectPointOntoAxis(aV, axis, minA, maxA);
            ProjectPointOntoAxis(bV, axis, minB, maxB);

            float overlap = IntervalOverlap(minA, maxA, minB, maxB);
            if (overlap <= 0.0f) return false; // separating axis found

			//select smallest overlap and its corresponding axis (for collision normal)
            if (overlap < bestOverlap) {
                bestOverlap = overlap;
                bestAxis = axis;
				bestAxisIndex = i;
            }
        }

        // Ensure normal points from A -> B
        Vec2 d = B.Position - A.Position;
        if (Dot(d, bestAxis) < 0.0f) bestAxis *= -1.0f;

        m.Normal = bestAxis;
        m.Penetration = bestOverlap;

        // --- build clipped manifold (up to 2 points) ---
        m.ContactCount = 0;

        Vec2 refV1, refV2;
        std::vector<Vec2> refVerts = (bestAxisIndex < 2) ? aV : bV; // reference box verts
        std::vector<Vec2> incVerts = (bestAxisIndex < 2) ? bV : aV; // incident box verts

        Vec2 refNormal = m.Normal;
        // If reference is B (bestIndex >= 2), refNormal should point from ref -> incident for clipping,
        // but your m.Normal is always A->B.
        // So flip for clipping when ref is B:
        if (bestAxisIndex >= 2) refNormal = refNormal * -1;

        // reference face
        GetFaceVertices(refVerts, refNormal, refV1, refV2);

        // incident face segment
        Vec2 incident[2];
        GetIncidentFace(incVerts, refNormal, incident);

        // side planes of the reference face
        Vec2 side = Normalize(refV2 - refV1);

        // Clip against first side
        Vec2 clip1[2];
        Vec2 clip2[2];

        int np = ClipSegmentToLine(clip1, incident, side * -1, Dot(side * -1, refV1));
        if (np < 2) return true; // still colliding, but degenerate manifold

        // Clip against second side
        np = ClipSegmentToLine(clip2, clip1, side, Dot(side, refV2));
        if (np < 2) return true;

        // Now keep points that are behind the reference face
        float refC = Dot(refNormal, refV1);

        for (int i = 0; i < 2; ++i) {
            float sep = Dot(refNormal, clip2[i]) - refC; // <= 0 means inside/behind face
            if (sep <= 0.0f) {
                // Convert back to your required A->B normal/    convention:
                Vec2 p = clip2[i];

                // If reference was B, we clipped in flipped space; the points are still world points,
                // but we must ensure we store contacts consistent with A/B ordering:
                // contacts are always world points, so no change needed.

                m.Contacts[m.ContactCount++] = p;
                if (m.ContactCount == 2) break;
            }
        }

        // If we clipped using ref=B (bestIndex>=2), we must flip back the manifold normal to A->B:
        if (bestAxisIndex >= 2) {
            // we already had m.Normal = bestAxis (A->B), so no change here
        }

        // If contact count ended up 0, fall back to your old midpoint approximation (rare but safe)
        if (m.ContactCount == 0) {
            Vec2 pA = SupportPoint(A, m.Normal * -1);
            Vec2 pB = SupportPoint(B, m.Normal);
            m.Contacts[0] = (pA + pB) * 0.5f;
            m.ContactCount = 1;
        }

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
        m.Contacts[0] = closestWorld;
		m.ContactCount = 1;
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
        m.Contacts[0] = A.Position + n * A.Shape.Radius;
        m.ContactCount = 1;
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
        m.Contacts[0] = (A.Position + B.Position) * 0.5f;
        m.ContactCount = 1;
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
        m.Contacts[0] = closest;
		m.ContactCount = 1;
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
        const float slop = 0.05f;

        float invMassSum = A.invMass + B.invMass;
        if (invMassSum <= 0) return;

        float corrMag = std::max(m.Penetration - slop, 0.0f) / invMassSum;
        Vec2 correction = m.Normal * (corrMag * percent);

        A.Position -= correction * A.invMass;
        B.Position += correction * B.invMass;
    }

}