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

	Body Space::addBody(f64 mass, f64 moment)
	{
		return Body{cpSpaceAddBody(handle(), cpBodyNew(mass, moment))};
	}

	Body Space::addStaticBody()
	{
		return Body{cpSpaceAddBody(handle(), cpBodyNewStatic())};
	}

	Body Space::addKinematicBody()
	{
		return Body{cpSpaceAddBody(handle(), cpBodyNewKinematic())};
	}
}
