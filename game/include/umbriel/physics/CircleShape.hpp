#pragma once

#include <umbriel/physics/Space.hpp>
#include <umbriel/physics/Shape.hpp>
#include <umbriel/physics/Body.hpp>

namespace umbriel::physics
{
	class CircleShape : public Shape
	{
	public:
		CircleShape(Space& space, Body& body, f64 radius, f64vec2 const& offset = {});

		~CircleShape() override = default;

		CircleShape(CircleShape&&) noexcept = default;

		CircleShape& operator=(CircleShape&&) noexcept = default;

		[[nodiscard]]
		f64vec2 offset() const;

		[[nodiscard]]
		f64 radius() const;
	};
}
