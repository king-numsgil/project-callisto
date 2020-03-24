#include <umbriel/physics/CircleShape.hpp>

namespace umbriel::physics
{
	CircleShape::CircleShape(Space& space, Body& body, f64 radius, const f64vec2& offset)
			: Shape{cpSpaceAddShape(space.handle(), cpCircleShapeNew(body.handle(), radius, {offset.x(), offset.y()}))}
	{}

	f64vec2 CircleShape::offset() const
	{
		cpVect offset = cpCircleShapeGetOffset(handle());
		return {offset.x, offset.y};
	}

	f64 CircleShape::radius() const
	{
		return cpCircleShapeGetRadius(handle());
	}
}
