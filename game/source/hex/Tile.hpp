#pragma once

#include "Coords.hpp"

namespace hex
{
	struct Tile
	{
		Tile(Tile const&) noexcept = delete;

		Tile& operator=(Tile const&) noexcept = delete;

		Tile(Tile&&) noexcept = default;

		Tile& operator=(Tile&&) noexcept = default;

		Tile(Axial const& Coord, u64 TypeIndex);

		Axial coord;
		u64 type_index;
	};
}
