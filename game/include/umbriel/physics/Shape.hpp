#pragma once

#include <umbriel/physics/ChipmunkHandle.hpp>
#include <chipmunk/chipmunk.h>

namespace umbriel::physics
{
	class Shape : public ChipmunkHandle<cpShape>
	{
	public:
		Shape();

		~Shape() override = default;
	};
}
