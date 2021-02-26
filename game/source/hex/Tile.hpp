#pragma once

#include "Feature.hpp"
#include "Coords.hpp"

namespace hex
{
	constexpr u64 FEATURES_COUNT = 10ull;

	struct Tile
	{
		Tile(Tile const&) noexcept = delete;

		Tile& operator=(Tile const&) noexcept = delete;

		Tile(Tile&&) noexcept = default;

		Tile& operator=(Tile&&) noexcept = default;

		Tile(Axial const& Coord, u64 TypeIndex);

		Axial coord;
		u64 type_index;
		array<optional<Feature>, FEATURES_COUNT> features;
	};
}
