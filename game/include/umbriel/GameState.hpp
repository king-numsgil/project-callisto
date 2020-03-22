#pragma once

#include <umbriel/State.hpp>

namespace umbriel
{
	class GameState : public State
	{
	public:
		explicit GameState(const std::weak_ptr<const StateManager>& manager);

		void draw_event(f32 delta) override;
	};
}
