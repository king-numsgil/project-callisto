#pragma once

#include <umbriel/physics/Space.hpp>
#include <umbriel/physics/Shape.hpp>
#include <umbriel/physics/Body.hpp>

namespace umbriel::physics
{
	class SegmentShape : public Shape
	{
	public:
		SegmentShape(Space& space, Body& body, f64vec2 a, f64vec2 b, f64 radius);

		~SegmentShape() override = default;

		SegmentShape(SegmentShape&&) noexcept = default;

		SegmentShape& operator=(SegmentShape&&) noexcept = default;

		void set_neighbors(f64vec2 prev, f64vec2 next);

		[[nodiscard]]
		f64vec2 get_a() const;

		[[nodiscard]]
		f64vec2 get_b() const;

		[[nodiscard]]
		f64vec2 normal() const;

		[[nodiscard]]
		f64 radius() const;
	};
}
