#pragma once

#include "Math.h"
#include "raylib.h"

namespace Core::Engine
{

	enum class ShapeType { Circle, AABB, OBB };

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
		Vec2 Contacts[2];   // up to 2 for 2D box contacts
		int ContactCount = 0;
	};

	class Body
	{
	public:
		Body(float mass, float rest, float friction, Vec2 initialPosition, float initialAngle, bool isStatic, Color color) : invMass(isStatic ? 0.0f : 1 / mass), Restitution(rest), Friction(friction), Position(initialPosition), Angle(initialAngle), FillColor(color) {};

		Color FillColor;
		Shape Shape{};
		Vec2 Position, Velocity;
		float invMass, invInertia, Angle, Restitution, Friction;
		float AngularVelociy = 0.0f;

		bool IsStatic() const { return invMass == 0.0f; }
	};
	 
	class Circle : public Body {
	public:
		Circle(float mass, float rest, float friction, bool isStatic, Vec2 initialPosition, float initialAngle, float radius, Color color) : Body(mass, rest, friction, initialPosition, initialAngle, isStatic, color) { 
			Shape.Type = ShapeType::Circle;
			Shape.Radius = radius;
			invInertia = isStatic ? 0.0f : 1.0f / (0.5f * mass * radius * radius);
		};

	};

	class Box : public Body {
	public:

		Box(float mass, float rest, float friction, bool isStatic, Vec2 initialPosition, float initialAngle, Vec2 dimensions, Color color) : Body(mass, rest, friction, initialPosition, initialAngle, isStatic, color) {
			//Shape.Type = (initialAngle == 0.0f && isStatic ? ShapeType::AABB : ShapeType::OBB);
			Shape.Type = ShapeType::OBB;
			Shape.Half = dimensions / 2;
			invInertia = isStatic ? 0.0f : 1.0f / (mass * (dimensions.x * dimensions.x + dimensions.y * dimensions.y) / 12.0f);
		};
	};


}