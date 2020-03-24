#include <Magnum/GL/Renderer.h>

#include <umbriel/Components.hpp>
#include <umbriel/GameState.hpp>
#include <umbriel/Log.hpp>

using namespace Corrade;
using namespace Magnum;

namespace umbriel
{
	GameState::GameState(const std::weak_ptr<const StateManager>& manager)
			: State(manager), _flat{FlatShader::Flag::Textured}
	{
		_player = _registry.create();
		_registry.assign<SpriteComponent>(_player).create("player_b.png", {30.f, 30.f});

		_proj = f64mat3::projection({1280, 768});
		_view = f64dcomp::translation({0., 0.});

		Debug{} << "GameState started!";
	}

	void GameState::draw_event(f32 delta)
	{
		GL::Renderer::disable(GL::Renderer::Feature::DepthTest);
		GL::Renderer::enable(GL::Renderer::Feature::Blending);

		_registry.view<SpriteComponent>().each(
				[this](SpriteComponent& sprite)
				{
					f64dcomp model{IdentityInit};
					_flat.bind_texture(sprite._texture)
							.set_transformation_projection_matrix(_proj * (_view * model).toMatrix());
					sprite._mesh.draw(_flat);
				}
		);

		_space.step(delta);
	}

	void GameState::key_press_event(State::KeyEvent& event)
	{
		if (event.key() == State::KeyEvent::Key::W && _accel <= .1)
		{
			_accel = 1000.;
			Debug{} << "Thrust on";
		}
	}

	void GameState::key_release_event(State::KeyEvent& event)
	{
		if (event.key() == State::KeyEvent::Key::W && _accel >= .1)
		{
			_accel = 0.;
			Debug{} << "Thrust off";
		}
	}
}
