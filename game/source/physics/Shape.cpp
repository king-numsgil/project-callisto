#include <umbriel/physics/Shape.hpp>

namespace umbriel::physics
{
	Shape::Shape() : ChipmunkHandle<cpShape>{nullptr, cpShapeFree}
	{}
}
