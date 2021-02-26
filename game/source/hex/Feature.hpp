#pragma once

#include "../Types.hpp"

namespace hex
{
	struct Feature
	{
		Feature(Feature const&) noexcept = delete;

		Feature& operator=(Feature const&) noexcept = delete;

		Feature(Feature&&) noexcept = default;

		Feature& operator=(Feature&&) noexcept = default;

		explicit Feature() = default;
	};
}
