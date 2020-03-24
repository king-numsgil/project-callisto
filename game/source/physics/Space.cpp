#include "umbriel/physics/Space.hpp"

namespace umbriel::physics
{
	Space::Space() : ChipmunkHandle<cpSpace>{cpSpaceNew(), cpSpaceFree}
	{
		cpSpaceSetGravity(handle(), {0., 0.});
	}

	void Space::step(f64 delta)
	{
		cpSpaceStep(handle(), delta);
	}
}
