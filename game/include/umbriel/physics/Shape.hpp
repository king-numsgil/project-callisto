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

	class Shape : public ChipmunkHandle<cpShape>
	{
	public:
		explicit Shape(cpShape* handle = nullptr);

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
	};
}
