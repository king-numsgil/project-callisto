#include <umbriel/GameState.hpp>

namespace umbriel
{
	GameState::GameState(const std::weak_ptr<const StateManager>& manager) : State(manager)
	{}

	void GameState::draw_event(f32 delta)
	{

	}
}
