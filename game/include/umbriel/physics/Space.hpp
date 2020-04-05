#pragma once

#include <umbriel/physics/ChipmunkHandle.hpp>
#include <chipmunk/chipmunk.h>

#include <umbriel/physics/Body.hpp>

namespace umbriel::physics
{
	class Space : public ChipmunkHandle<cpSpace>
	{
	public:
		Space();

		~Space() override = default;

		Space(Space&&) noexcept = default;

		Space& operator=(Space&&) noexcept = default;

		void step(f64 delta);

		Body addBody(f64 mass, f64 moment);

		Body addStaticBody();

		Body addKinematicBody();
	};
}
