#pragma once

#include <umbriel/physics/Shape.hpp>

namespace umbriel::physics
{
	class CircleShape : public Shape
	{
	public:
		explicit CircleShape(cpShape* shape = nullptr);

		~CircleShape() override = default;

		CircleShape(CircleShape&&) noexcept = default;

		CircleShape& operator=(CircleShape&&) noexcept = default;

		[[nodiscard]]
		f64vec2 offset() const;

		[[nodiscard]]
		f64 radius() const;
	};
}
