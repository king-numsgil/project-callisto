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

		~BoxShape() override = default;

		BoxShape(BoxShape&&) noexcept = default;

		BoxShape& operator=(BoxShape&&) noexcept = default;

		[[nodiscard]]
		f64 radius() const;
	};
}
