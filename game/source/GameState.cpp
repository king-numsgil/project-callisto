#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/Transform.h>
#include <Magnum/Primitives/Square.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/Trade/MeshData2D.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/ImageView.h>
#include <Magnum/GL/Buffer.h>

#include <umbriel/GameState.hpp>
#include <umbriel/Log.hpp>

using namespace Corrade;
using namespace Magnum;

namespace umbriel
{
	GameState::GameState(const std::weak_ptr<const StateManager>& manager)
			: State(manager), _flat{Shaders::Flat2D::Flag::Textured}, _player{}, _sprite{}
	{
		PluginManager::Manager<Trade::AbstractImporter> plugins;
		auto importer = plugins.loadAndInstantiate("PngImporter");
		if (!importer) Fatal{} << "Cannot load PngImporter plugin";

		Utility::Resource const rs{"UmbrielSprites"};
		if (!importer->openData(rs.getRaw("player_b.png")))
			Fatal{} << "Cannot load 'player_b.png'";

		auto image = importer->image2D(0);
		CORRADE_INTERNAL_ASSERT(image);
		_player.setWrapping(GL::SamplerWrapping::ClampToEdge)
				.setMagnificationFilter(GL::SamplerFilter::Nearest)
				.setMinificationFilter(GL::SamplerFilter::Nearest)
				.setStorage(1, GL::textureFormat(image->format()), image->size())
				.setSubImage(0, {}, *image);

		Trade::MeshData2D quad = Primitives::squareSolid(Primitives::SquareTextureCoords::Generate);
		MeshTools::transformVectorsInPlace(f32mat3::scaling({30.f, 30.f}), quad.positions(0));
		GL::Buffer sprite_data{};
		sprite_data.setData(MeshTools::interleave(quad.positions(0), quad.textureCoords2D(0)));
		_sprite.setPrimitive(quad.primitive())
				.setCount(quad.positions(0).size())
				.addVertexBuffer(std::move(sprite_data), 0, Shaders::Flat2D::Position{},
				                 Shaders::Flat2D::TextureCoordinates{});

		_proj = f32mat3::projection({1280, 768});
		_view = f32mat3::translation({0.f, 0.f});

		Debug{} << "GameState started!";
	}

	void GameState::draw_event(f32)
	{
		GL::Renderer::disable(GL::Renderer::Feature::DepthTest);
		GL::Renderer::enable(GL::Renderer::Feature::Blending);
		_flat.bindTexture(_player)
				.setTransformationProjectionMatrix(_proj * _view);
		_sprite.draw(_flat);
	}
}
