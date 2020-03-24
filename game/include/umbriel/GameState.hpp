#pragma once

#include <Magnum/Shaders/Flat.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/Mesh.h>
#include <entt/entt.hpp>

#include <umbriel/physics/Space.hpp>
#include <umbriel/State.hpp>

namespace umbriel
{
	class GameState : public State
	{
		Magnum::Shaders::Flat2D _flat;
		physics::Space _space{};
		//b2World _world{{0.f, 0.f}};
		entt::registry _registry{};
		entt::entity _player{};

		f32mat3 _proj{IdentityInit};
		f32dcomp _view{IdentityInit};

		f32 _accel{0.f};

	public:
		explicit GameState(const std::weak_ptr<const StateManager>& manager);

		void draw_event(f32 delta) override;

		virtual void key_press_event(KeyEvent& event) override;

		virtual void key_release_event(KeyEvent& event) override;
	};
}
