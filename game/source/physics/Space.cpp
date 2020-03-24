#include "umbriel/physics/Space.hpp"

namespace umbriel::physics
{
	Space::Space() : ChipmunkHandle<cpSpace>(cpSpaceNew(), cpSpaceFree)
	{
		cpSpaceSetGravity(handle(), {0.f, 0.f});
	}

	void Space::step(f32 delta)
	{
		cpSpaceStep(handle(), delta);
	}
}
