#pragma once

#include "Math.h"

#define STATIC_BODY 0.0f

namespace Core::Engine
{

	enum class ShapeType { Circle, AABB };

	struct Shape {

		ShapeType Type;
		union
		{
			float Radius;
			Vec2 Half;
		};
	};

	struct Manifold
	{
		int ia = -1, ib = -1;
		float Penetration = 0;
		Vec2 Normal;
		Vec2 ContactPoint;
	};

	class Body
	{
	public:
		Body(float mass, float rest, float friction, Vec2 initialPosition, bool isStatic) : invMass(isStatic ? 0.0f : 1 / mass), Restitution(rest), Friction(friction), Position(initialPosition) {};

		Shape Shape{};
		Vec2 Position, Velocity, Force;
		float invMass, Restitution, Friction;

		bool IsStatic() const { return invMass == 0.0f; }
	};

	class Circle : public Body {
	public:
		Circle(float mass, float rest, float friction, bool isStatic, Vec2 initialPosition, float radius) : Body(mass, rest, friction, initialPosition, isStatic) { Shape.Type = ShapeType::Circle, Shape.Radius = radius; };

	};

	class Box : public Body {
	public:
		Box(float mass, float rest, float friction, bool isStatic, Vec2 initialPosition, Vec2 dimensions) : Body(mass, rest, friction, initialPosition, isStatic) { Shape.Type = ShapeType::AABB, Shape.Half = dimensions/2; };
	};


}