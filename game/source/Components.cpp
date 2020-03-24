#include <Magnum/Trade/AbstractImporter.h>
#include <Corrade/Containers/Optional.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/Transform.h>
#include <Magnum/Primitives/Square.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/Trade/MeshData2D.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/ImageView.h>

#include <umbriel/Components.hpp>
#include <umbriel/Log.hpp>

using namespace Corrade;
using namespace Magnum;

namespace umbriel
{
	void SpriteComponent::create(string const& filename, f32vec2 const& size)
	{
		PluginManager::Manager<Trade::AbstractImporter> plugins;
		auto importer = plugins.loadAndInstantiate("StbImageImporter");
		if (!importer) Fatal{} << "Cannot load StbImageImporter plugin";

		Utility::Resource const rs{"UmbrielSprites"};
		if (!importer->openData(rs.getRaw(filename)))
			Fatal{} << "Cannot load" << filename.c_str();

		auto image = importer->image2D(0);
		CORRADE_INTERNAL_ASSERT(image);
		_texture = GL::Texture2D{};
		_texture.setWrapping(GL::SamplerWrapping::ClampToEdge)
				.setMagnificationFilter(GL::SamplerFilter::Nearest)
				.setMinificationFilter(GL::SamplerFilter::Nearest)
				.setStorage(1, GL::textureFormat(image->format()), image->size())
				.setSubImage(0, {}, *image);

		Trade::MeshData2D quad = Primitives::squareSolid(Primitives::SquareTextureCoords::Generate);
		MeshTools::transformVectorsInPlace(f32mat3::scaling(size), quad.positions(0));
		_buffer = GL::Buffer{};
		_buffer.setData(MeshTools::interleave(quad.positions(0), quad.textureCoords2D(0)));

		_mesh = GL::Mesh{};
		_mesh.setPrimitive(quad.primitive())
				.setCount(quad.positions(0).size())
				.addVertexBuffer(std::move(_buffer), 0, Shaders::Flat2D::Position{},
				                 Shaders::Flat2D::TextureCoordinates{});
	}
}
