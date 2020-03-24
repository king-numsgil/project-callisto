#include <umbriel/physics/SegmentShape.hpp>

namespace umbriel::physics
{
	SegmentShape::SegmentShape(Space& space, Body& body, f64vec2 a, f64vec2 b, f64 radius)
			: Shape{cpSpaceAddShape(space.handle(), cpSegmentShapeNew(body.handle(), {a.x(), a.y()}, {b.x(), b.y()}, radius))}
	{}

	void SegmentShape::set_neighbors(f64vec2 prev, f64vec2 next)
	{
		cpSegmentShapeSetNeighbors(handle(), {prev.x(), prev.y()}, {next.x(), next.y()});
	}

	f64vec2 SegmentShape::get_a() const
	{
		cpVect a = cpSegmentShapeGetA(handle());
		return {a.x, a.y};
	}

	f64vec2 SegmentShape::get_b() const
	{
		cpVect b = cpSegmentShapeGetB(handle());
		return {b.x, b.y};
	}

	f64vec2 SegmentShape::normal() const
	{
		cpVect n = cpSegmentShapeGetNormal(handle());
		return {n.x, n.y};
	}

	f64 SegmentShape::radius() const
	{
		return cpSegmentShapeGetRadius(handle());
	}
}
