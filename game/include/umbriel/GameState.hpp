#pragma once

#include <Magnum/Shaders/Flat.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/Mesh.h>

#include <umbriel/State.hpp>

namespace umbriel
{
	class GameState : public State
	{
		Magnum::Shaders::Flat2D _flat;
		Magnum::GL::Texture2D _player;
		Magnum::GL::Mesh _sprite;

		f32mat3 _proj{IdentityInit}, _view{IdentityInit};

	public:
		explicit GameState(const std::weak_ptr<const StateManager>& manager);

		void draw_event(f32 delta) override;
	};
}
