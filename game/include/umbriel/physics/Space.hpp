#pragma once

#include <umbriel/physics/ChipmunkHandle.hpp>
#include <chipmunk/chipmunk.h>

namespace umbriel::physics
{
	class Space : public ChipmunkHandle<cpSpace>
	{
	public:
		Space();

		~Space() override = default;

		void step(f32 delta);
	};
}
