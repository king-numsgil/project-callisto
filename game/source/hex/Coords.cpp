#include <cmath>

#include "Coords.hpp"

using namespace std;

namespace hex
{
	Axial::Axial(const Cubic& cubic) : Axial{cubic.x(), cubic.z()}
	{}

	Axial::operator Cubic() const
	{ return Cubic{q(), -q() - r(), r()}; }

	Cubic::Cubic(const Axial& axial) : Cubic{axial.q(), 0, axial.r()}
	{ y() = -x() - z(); }

	Cubic::operator Axial() const
	{ return Axial{x(), z()}; }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"

	f32vec2 Axial::to_position(f32 radius) const
	{
		if constexpr (FLAT_TOPPED)
		{
			return {
					radius * (3.f / 2.f * f32(q())),
					radius * (sqrtf(3.f) / 2.f * f32(q()) + sqrtf(3.f) * f32(r()))
			};
		}
		else
		{
			return {
					radius * (sqrtf(3.f) * f32(q()) + sqrtf(3.f) / 2.f * f32(r())),
					radius * (3.f / 2.f * f32(r()))
			};
		}
	}

	Axial Axial::from_position(f32vec2 const& position, f32 radius)
	{
		f32 q = FLAT_TOPPED
		        ? (2.f / 3.f * position.x()) / radius
		        : (sqrtf(3.f) / 3.f * position.x() - 1.f / 3.f * position.y()) / radius;
		f32 r = FLAT_TOPPED
		        ? (-1.f / 3.f * position.x() + sqrtf(3.f) / 3.f * position.y()) / radius
		        : (2.f / 3.f * position.y()) / radius;

		f32vec3 cube{q, -q - r, r};
		f32vec3 diff{
				abs(roundf(cube.x()) - cube.x()),
				abs(roundf(cube.y()) - cube.y()),
				abs(roundf(cube.z()) - cube.z()),
		};

		cube = round(cube);
		if (diff.x() > diff.y() && diff.x() > diff.z())
			cube.x() = -cube.y() - cube.z();
		else if (diff.y() > diff.z())
			cube.y() = -cube.x() - cube.z();
		else
			cube.z() = -cube.x() - cube.y();

		return Axial{Cubic{
				static_cast<i32>(cube.x()),
				static_cast<i32>(cube.y()),
				static_cast<i32>(cube.z())
		}};
	}

#pragma clang diagnostic pop
}
