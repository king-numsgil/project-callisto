#pragma once

#include <umbriel/physics/ChipmunkHandle.hpp>
#include <Corrade/Containers/Reference.h>
#include <Corrade/Containers/Pointer.h>
#include <chipmunk/chipmunk.h>
#include <vector>

#include <umbriel/physics/SegmentShape.hpp>
#include <umbriel/physics/CircleShape.hpp>
#include <umbriel/physics/BoxShape.hpp>

namespace umbriel::physics
{
	enum class BodyType
	{
		Dynamic = CP_BODY_TYPE_DYNAMIC,
		Kinematic = CP_BODY_TYPE_KINEMATIC,
		Static = CP_BODY_TYPE_STATIC,
	};

	class Body : public ChipmunkHandle<cpBody>
	{
	public:
		explicit Body(cpBody* body = nullptr);

		~Body() override = default;

		Body(Body&&) noexcept = default;

		Body& operator=(Body&&) noexcept = default;

		[[nodiscard]]
		f64dcomp transform() const;

		void activate();

		void sleep();

		[[nodiscard]]
		bool sleeping() const;

		[[nodiscard]]
		BodyType type() const;

		void type(BodyType value);

		[[nodiscard]]
		f64 mass() const;

		void mass(f64 value);

		[[nodiscard]]
		f64 moment() const;

		void moment(f64 value);

		[[nodiscard]]
		f64vec2 position() const;

		void position(f64vec2 value);

		[[nodiscard]]
		f64vec2 center_of_gravity() const;

		void center_of_gravity(f64vec2 value);

		[[nodiscard]]
		f64vec2 velocity() const;

		void velocity(f64vec2 value);

		[[nodiscard]]
		f64vec2 force() const;

		void force(f64vec2 value);

		[[nodiscard]]
		f64 angular_velocity() const;

		void angular_velocity(f64 value);

		[[nodiscard]]
		f64 torque() const;

		void torque(f64 value);

		[[nodiscard]]
		f64vec2 local_to_world(f64vec2 point) const;

		[[nodiscard]]
		f64vec2 world_to_local(f64vec2 point) const;

		void apply_force_at_world_point(f64vec2 force, f64vec2 point);

		void apply_force_at_local_point(f64vec2 force, f64vec2 point);

		void apply_impulse_at_world_point(f64vec2 impulse, f64vec2 point);

		void apply_impulse_at_local_point(f64vec2 impulse, f64vec2 point);

		[[nodiscard]]
		f64 kinetic_energy() const;

		Corrade::Containers::Reference<BoxShape> addBoxShape(f64 width, f64 height, f64 radius);

		Corrade::Containers::Reference<CircleShape> addCircleShape(f64 radius, f64vec2 offset = {});

		Corrade::Containers::Reference<SegmentShape> addSegmentShape(f64vec2 a, f64vec2 b, f64 radius);

	private:
		std::vector<Corrade::Containers::Pointer<Shape>> _shapes{};
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
