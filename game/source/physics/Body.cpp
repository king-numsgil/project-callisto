#include <umbriel/physics/Body.hpp>

namespace umbriel::physics
{
	Body::Body(Space& space, f64 mass, f64 moment)
			: ChipmunkHandle<cpBody>{cpSpaceAddBody(space.handle(), cpBodyNew(mass, moment)), cpBodyFree}
	{}

	f64dcomp Body::transform() const
	{
		auto pos = cpBodyGetPosition(handle());
		return f64dcomp::translation({pos.x, pos.y}) *
		       f64dcomp::rotation(f64rad{cpBodyGetAngle(handle())});
	}
}
