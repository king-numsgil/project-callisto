#pragma once

#include <umbriel/physics/Space.hpp>

namespace umbriel::physics
{
	class Body : public ChipmunkHandle<cpBody>
	{
	public:
		Body(Space& space, f64 mass, f64 moment);

		~Body() override = default;

		[[nodiscard]]
		f64dcomp transform() const;
	};

	static inline f32 moment_for_circle(f32 mass, f32 r1, f32 r2, f32vec2 offset)
	{
		return cpMomentForCircle(mass, r1, r2, {offset.x(), offset.y()});
	}

	static inline f32 moment_for_segment(f32 mass, f32vec2 a, f32vec2 b, f32 radius)
	{
		return cpMomentForSegment(mass, {a.x(), a.y()}, {b.x(), b.y()}, radius);
	}

	static inline f32 moment_for_box(f32 mass, f32 width, f32 height)
	{
		return cpMomentForBox(mass, width, height);
	}
}
