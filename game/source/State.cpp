#include <umbriel/State.hpp>

namespace umbriel
{
	void StateManager::push_state(State* s)
	{
		insert(s, current_state());
	}

	State* StateManager::pop_state()
	{
		if (State* ret = current_state(); ret)
		{
			cut(ret);
			return ret;
		}

		return nullptr;
	}

	void StateManager::draw_event(f32 delta)
	{
		for (State* s = first(); s; s = s->next_state())
			s->draw_event(delta);
	}

	void StateManager::key_press_event(StateManager::KeyEvent& event)
	{
		if (current_state() != nullptr)
			current_state()->key_press_event(event);
	}

	void StateManager::key_release_event(StateManager::KeyEvent& event)
	{
		if (current_state() != nullptr)
			current_state()->key_release_event(event);
	}

	void StateManager::mouse_press_event(StateManager::MouseEvent& event)
	{
		if (current_state() != nullptr)
			current_state()->mouse_press_event(event);
	}

	void StateManager::mouse_release_event(StateManager::MouseEvent& event)
	{
		if (current_state() != nullptr)
			current_state()->mouse_release_event(event);
	}

	void StateManager::mouse_move_event(StateManager::MouseMoveEvent& event)
	{
		if (current_state() != nullptr)
			current_state()->mouse_move_event(event);
	}

	void StateManager::mouse_scroll_event(StateManager::MouseScrollEvent& event)
	{
		if (current_state() != nullptr)
			current_state()->mouse_scroll_event(event);
	}

	void StateManager::text_input_event(StateManager::TextInputEvent& event)
	{
		if (current_state() != nullptr)
			current_state()->text_input_event(event);
	}
}
