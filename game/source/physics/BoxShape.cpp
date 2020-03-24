#include <umbriel/physics/BoxShape.hpp>

namespace umbriel::physics
{
	BoxShape::BoxShape(Space& space, Body& body, f64 width, f64 height, f64 radius)
	: Shape{cpSpaceAddShape(space.handle(), cpBoxShapeNew(body.handle(), width, height, radius))}
	{}

	f64 BoxShape::radius() const
	{
		return cpPolyShapeGetRadius(handle());
	}
}
