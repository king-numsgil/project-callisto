#pragma once

#include <umbriel/physics/ChipmunkHandle.hpp>
#include <chipmunk/chipmunk.h>

namespace umbriel::physics
{
	struct BoundingBox
	{
		BoundingBox(f64 left, f64 bottom, f64 right, f64 top)
				: _left{left}, _bottom{bottom}, _right{right}, _top{top}
		{}

		f64 _left, _bottom, _right, _top;
	};

	class Space : public ChipmunkHandle<cpSpace>
	{
	public:
		Space();

		~Space() override = default;

		Space(Space&&) noexcept = default;

		Space& operator=(Space&&) noexcept = default;

		void step(f64 delta);
	};
}
