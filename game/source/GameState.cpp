#include <Magnum/GL/Renderer.h>

#include <umbriel/physics/BoxShape.hpp>
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
		auto& playerBody = _registry.assign<BodyComponent>(_player);
		playerBody._body = _space.addBody(10, physics::moment_for_box(10, 30, 30));
		playerBody._shapes.push_back(std::make_unique<physics::BoxShape>(_space, playerBody._body, 30, 30, 0));
		playerBody._body.type(physics::BodyType::Dynamic);

		_proj = f64mat3::projection({1280, 768});
		_view = f64dcomp::translation({0., 0.});

		Debug{} << "GameState started!";
	}

	void GameState::draw_event(f32 delta)
	{
		GL::Renderer::disable(GL::Renderer::Feature::DepthTest);
		GL::Renderer::enable(GL::Renderer::Feature::Blending);

		_registry.get<BodyComponent>(_player)._body.apply_force_at_local_point({0., _accel}, {0., 0.});

		_registry.view<BodyComponent, SpriteComponent>().each(
				[this](BodyComponent& body, SpriteComponent& sprite)
				{
					f64dcomp model = body._body.transform();
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
