#pragma once

#include <Magnum/Platform/GlfwApplication.h>
#include <Corrade/Containers/LinkedList.h>
#include <umbriel/Types.hpp>
#include <utility>
#include <memory>

namespace umbriel
{
	class StateManager;

	class State : private Corrade::Containers::LinkedListItem<State, StateManager>
	{
		friend Corrade::Containers::LinkedListItem<State, StateManager>;
		friend Corrade::Containers::LinkedList<State>;

	public:
		using KeyEvent = Magnum::Platform::GlfwApplication::KeyEvent;
		using MouseEvent = Magnum::Platform::GlfwApplication::MouseEvent;
		using MouseMoveEvent = Magnum::Platform::GlfwApplication::MouseMoveEvent;
		using TextInputEvent = Magnum::Platform::GlfwApplication::TextInputEvent;
		using MouseScrollEvent = Magnum::Platform::GlfwApplication::MouseScrollEvent;

		inline explicit State(std::weak_ptr<StateManager const> manager)
				: _manager{std::move(manager)}
		{}

		~State() override = default;

		[[nodiscard]]
		std::shared_ptr<StateManager const> manager() const
		{ return _manager.lock(); }

		[[nodiscard]]
		State const* next_state() const
		{ return next(); }

		[[nodiscard]]
		State* next_state()
		{ return next(); }

		virtual void draw_event(f32 delta) = 0;

		virtual void key_press_event(KeyEvent&)
		{}

		virtual void key_release_event(KeyEvent&)
		{}

		virtual void mouse_press_event(MouseEvent&)
		{}

		virtual void mouse_release_event(MouseEvent&)
		{}

		virtual void mouse_move_event(MouseMoveEvent&)
		{}

		virtual void mouse_scroll_event(MouseScrollEvent&)
		{}

		virtual void text_input_event(TextInputEvent&)
		{}

	private:
		std::weak_ptr<StateManager const> _manager;
	};

	class StateManager
			: protected std::enable_shared_from_this<StateManager>, private Corrade::Containers::LinkedList<State>
	{
		friend Corrade::Containers::LinkedListItem<State, StateManager>;
		friend Corrade::Containers::LinkedList<State>;

	public:
		using KeyEvent = Magnum::Platform::GlfwApplication::KeyEvent;
		using MouseEvent = Magnum::Platform::GlfwApplication::MouseEvent;
		using MouseMoveEvent = Magnum::Platform::GlfwApplication::MouseMoveEvent;
		using TextInputEvent = Magnum::Platform::GlfwApplication::TextInputEvent;
		using MouseScrollEvent = Magnum::Platform::GlfwApplication::MouseScrollEvent;

		StateManager() = default;

		~StateManager() = default;

		StateManager(StateManager const&) = delete;

		StateManager& operator=(StateManager const&) = delete;

		StateManager(StateManager&&) = default;

		StateManager& operator=(StateManager&&) = default;

		inline std::shared_ptr<StateManager const> shared_ptr() const
		{ return shared_from_this(); }

		inline std::shared_ptr<StateManager> shared_ptr()
		{ return shared_from_this(); }

		inline std::weak_ptr<StateManager const> weak_ptr() const
		{ return weak_from_this(); }

		inline std::weak_ptr<StateManager> weak_ptr()
		{ return weak_from_this(); }

		State const* current_state() const
		{ return first(); }

		State* current_state()
		{ return first(); }

		void draw_event(f32 delta);

		void key_press_event(KeyEvent& event);

		void key_release_event(KeyEvent& event);

		void mouse_press_event(MouseEvent& event);

		void mouse_release_event(MouseEvent& event);

		void mouse_move_event(MouseMoveEvent& event);

		void mouse_scroll_event(MouseScrollEvent& event);

		void text_input_event(TextInputEvent& event);
	};
}
