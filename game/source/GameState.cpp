#include <Magnum/GL/Renderer.h>

#include <umbriel/Components.hpp>
#include <umbriel/GameState.hpp>
#include <umbriel/Log.hpp>

using namespace Corrade;
using namespace Magnum;

namespace umbriel
{
	GameState::GameState(const std::weak_ptr<const StateManager>& manager)
			: State(manager), _flat{Shaders::Flat2D::Flag::Textured}
	{
		b2BodyDef playerBD{};
		playerBD.angularDamping = .2f;
		playerBD.linearDamping = .3f;
		playerBD.type = b2_kinematicBody;

		b2PolygonShape playerShape{};
		playerShape.SetAsBox(3.f, 3.f);

		b2FixtureDef playerFix{};
		playerFix.shape = &playerShape;
		playerFix.density = 1.f;
		playerFix.friction = .3f;

		_player = _registry.create();
		_registry.assign<SpriteComponent>(_player).create("player_b.png", {30.f, 30.f});
		_registry.assign<BodyComponent>(_player)._body = _world.CreateBody(&playerBD);
		_registry.get<BodyComponent>(_player)._body->CreateFixture(&playerFix);

		_proj = f32mat3::projection({1280, 768});
		_view = f32dcomp::translation({0.f, 0.f});

		Debug{} << "GameState started!";
	}

	void GameState::draw_event(f32 delta)
	{
		GL::Renderer::disable(GL::Renderer::Feature::DepthTest);
		GL::Renderer::enable(GL::Renderer::Feature::Blending);

		_registry.view<BodyComponent, SpriteComponent>().each(
				[this](BodyComponent& body, SpriteComponent& sprite)
				{
					if (body._body != nullptr)
					{
						f32dcomp model{body._body->GetTransform()};
						_flat.bindTexture(sprite._texture)
								.setTransformationProjectionMatrix(_proj * (_view * model).toMatrix());
						sprite._mesh.draw(_flat);
					}
				}
		);

		_world.Step(delta, 6, 3);
	}
}
