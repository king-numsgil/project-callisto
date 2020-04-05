#include <umbriel/physics/CircleShape.hpp>

namespace umbriel::physics
{
	CircleShape::CircleShape(cpShape* shape) : Shape{shape}
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
