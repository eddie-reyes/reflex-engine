#pragma once

#include "Body.h"

namespace Core::Engine 
{

    inline void ApplyImpulse(Body& A, Body& B, const Manifold& m) {

        Vec2 ra = m.ContactPoint - A.Position;
        Vec2 rb = m.ContactPoint - B.Position;

        Vec2 rv = (B.Velocity + Cross(B.AngularVelociy, rb)) - (A.Velocity + Cross(A.AngularVelociy, ra));
        float VelocityAlongNormal = Dot(rv, m.Normal);

        if (VelocityAlongNormal > 0) return;  //separating

        float e = std::min(A.Restitution, B.Restitution);
        if (std::fabs(VelocityAlongNormal) < 0.5) e = 0.0f;

        float raCrossN = Cross(ra, m.Normal);
        float rbCrossN = Cross(rb, m.Normal);

        float invMassSum = A.invMass + B.invMass + (raCrossN * raCrossN) * A.invInertia + (rbCrossN * rbCrossN) * B.invInertia;
        if (invMassSum <= 0) return;

        float j = -(1.0f + e) * VelocityAlongNormal;
        j /= invMassSum;

        Vec2 impulse = m.Normal * j;
        A.Velocity -= impulse * A.invMass;
        B.Velocity += impulse * B.invMass;

        A.AngularVelociy -= Cross(ra, impulse) * A.invInertia;
        B.AngularVelociy += Cross(rb, impulse) * B.invInertia;

        //Friction
        rv = (B.Velocity + Cross(B.AngularVelociy, rb)) - (A.Velocity + Cross(A.AngularVelociy, ra));
        Vec2 tangent = rv - m.Normal * Dot(rv, m.Normal);
        float tLen = Len(tangent);
        if (tLen > MIN_VEC_LEN) tangent = Normalize(tangent);
        else return;

        float raCrossT = Cross(ra, tangent);
        float rbCrossT = Cross(rb, tangent);

        invMassSum = A.invMass + B.invMass + (raCrossT * raCrossT) * A.invInertia + (rbCrossT * rbCrossT) * B.invInertia;

        float jt = -Dot(rv, tangent);
        jt /= invMassSum;

        float mu = std::sqrt(A.Friction * B.Friction);

		float jtClamped = std::clamp(jt, -j * mu, j * mu);

        Vec2 FrictionImpulse = tangent * jtClamped;
  
        A.Velocity -= FrictionImpulse * A.invMass;
        B.Velocity += FrictionImpulse * B.invMass;

        A.AngularVelociy -= Cross(ra, FrictionImpulse) * A.invInertia;
        B.AngularVelociy += Cross(rb, FrictionImpulse) * B.invInertia;
    }
}