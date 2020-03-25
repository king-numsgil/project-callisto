#pragma once

#include <umbriel/physics/Space.hpp>

namespace umbriel::physics
{
	class Shape : public ChipmunkHandle<cpShape>
	{
	public:
		Shape();

		~Shape() override = default;

		Shape(Shape&&) noexcept = default;

		Shape& operator=(Shape&&) noexcept = default;

		BoundingBox update(f64dcomp transform);

		[[nodiscard]]
		f64 mass() const;

		void mass(f64 value);

		[[nodiscard]]
		f64 density() const;

		void density(f64 value);

		[[nodiscard]]
		f64 moment() const;

		[[nodiscard]]
		f64 area() const;

		[[nodiscard]]
		f64vec2 center_of_gravity() const;

		[[nodiscard]]
		BoundingBox bounding_box() const;

		[[nodiscard]]
		bool sensor() const;

		void sensor(bool value);

		[[nodiscard]]
		f64 elasticity() const;

		void elasticity(f64 value);

		[[nodiscard]]
		f64 friction() const;

		void friction(f64 value);

		[[nodiscard]]
		f64vec2 surface_velocity() const;

		void surface_velocity(f64vec2 value);

		[[nodiscard]]
		u64 collision_type() const;

		void collision_type(u64 value);

	protected:
		explicit Shape(cpShape* handle);
	};
}
