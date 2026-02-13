#pragma once

#include "Body.h"

namespace Core::Engine 
{

    inline void ApplyImpulse(Body& A, Body& B, const Manifold& m) {
        Vec2 rv = B.Velocity - A.Velocity;
        float VelocityAlongNormal = Dot(rv, m.Normal);

        if (VelocityAlongNormal > 0) return;  //separating

        float e = std::min(A.Restitution, B.Restitution);

        float invMassSum = A.invMass + B.invMass;
        if (invMassSum <= 0) return;

        float j = -(1.0f + e) * VelocityAlongNormal;
        j /= invMassSum;

        Vec2 impulse = m.Normal * j;
        A.Velocity -= impulse * A.invMass;
        B.Velocity += impulse * B.invMass;

        //Friction
        rv = B.Velocity - A.Velocity;
        Vec2 tangent = rv - m.Normal * Dot(rv, m.Normal);
        float tLen = Len(tangent);
        if (tLen > MIN_VEC_LEN) tangent = Normalize(tangent);
        else return;

        float jt = -Dot(rv, tangent);
        jt /= invMassSum;

        float mu = std::sqrt(A.Friction * B.Friction);

		float jtClamped = std::clamp(jt, -j * mu, j * mu);

        Vec2 FrictionImpulse = tangent * jtClamped;
  
        A.Velocity -= FrictionImpulse * A.invMass;
        B.Velocity += FrictionImpulse * B.invMass;
    }
}