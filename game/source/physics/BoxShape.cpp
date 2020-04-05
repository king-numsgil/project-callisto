#include <umbriel/physics/BoxShape.hpp>

namespace umbriel::physics
{
	BoxShape::BoxShape(cpShape* shape) : Shape{shape}
	{}

	f64 BoxShape::radius() const
	{
		return cpPolyShapeGetRadius(handle());
	}
}
