#pragma once

#include <umbriel/physics/Shape.hpp>

namespace umbriel::physics
{
	class BoxShape : public Shape
	{
	public:
		explicit BoxShape(cpShape* shape = nullptr);

		~BoxShape() override = default;

		BoxShape(BoxShape&&) noexcept = default;

		BoxShape& operator=(BoxShape&&) noexcept = default;

		[[nodiscard]]
		f64 radius() const;
	};
}
