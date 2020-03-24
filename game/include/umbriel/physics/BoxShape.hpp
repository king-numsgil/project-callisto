#pragma once

#include <umbriel/physics/Space.hpp>
#include <umbriel/physics/Shape.hpp>
#include <umbriel/physics/Body.hpp>

namespace umbriel::physics
{
	class BoxShape : public Shape
	{
	public:
		BoxShape(Space& space, Body& body, f64 width, f64 height, f64 radius);

		[[nodiscard]]
		f64 radius() const;
	};
}
