#include "Tile.hpp"

namespace hex
{
	Tile::Tile(Axial const& Coord, u64 TypeIndex)
			: coord{Coord}, type_index{TypeIndex}, features{nullopt}
	{}
}
