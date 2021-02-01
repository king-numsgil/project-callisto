#include <Magnum/Trade/AbstractImporter.h>
#include <Corrade/Containers/Optional.h>
#include <Magnum/MeshTools/Transform.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/ImageView.h>
#include <nlohmann/json.hpp>
#include <fstream>

#include "HexGrid.hpp"

using namespace Magnum;

using json = nlohmann::json;

constexpr Trade::MeshAttributeData AttributeData2DTextureCoords[]{
		Trade::MeshAttributeData{Trade::MeshAttribute::Position, VertexFormat::Vector2,
		                         0, 0, 2 * sizeof(f32vec2)},
		Trade::MeshAttributeData{Trade::MeshAttribute::TextureCoordinates, VertexFormat::Vector2,
		                         sizeof(f32vec2), 0, 2 * sizeof(f32vec2)}
};

Trade::MeshData hexSolid(f32 radius = 1.f)
{
	const u32 segments = 6;

	Containers::Array<Trade::MeshAttributeData> attributes
			= Trade::meshAttributeDataNonOwningArray(AttributeData2DTextureCoords);

	const std::size_t stride = attributes[0].stride();
	Containers::Array<char> vertexData{stride * (segments + 2)};

	Containers::StridedArrayView1D<Vector2> positions
			{vertexData,
			 reinterpret_cast<f32vec2*>(vertexData.begin()),
			 segments + 2, std::ptrdiff_t(stride)};
	positions[0] = {};
	/* Points on the circle. The first/last point is here twice to close the
	   circle properly. */
	const f32rad angleIncrement(f32const::tau() / segments);
	for (u32 i = 0; i != segments + 1; ++i)
	{
		const f32rad angle(f32(i) * angleIncrement);
		const auto[sin, cos] = Math::sincos(angle);
		positions[i + 1] = {cos, sin};
	}

	Containers::StridedArrayView1D<Vector2> textureCoords
			{vertexData,
			 reinterpret_cast<f32vec2*>(vertexData.begin() + sizeof(f32vec2)),
			 positions.size(), std::ptrdiff_t(stride)};
	for (std::size_t i = 0; i != positions.size(); ++i)
		textureCoords[i] = positions[i] * .5f + f32vec2{.5f};

	MeshTools::transformPointsInPlace(f32mat3::scaling(f32vec2{radius}), positions);
	return Trade::MeshData
			{MeshPrimitive::TriangleFan, std::move(vertexData), std::move(attributes), u32(positions.size())};
}

namespace hex
{
	Axial::Axial(const Cubic& cubic) : Axial{cubic.x(), cubic.z()}
	{}

	Axial::operator Cubic() const
	{ return Cubic{q(), -q() - r(), r()}; }

	Cubic::Cubic(const Axial& axial) : Cubic{axial.q(), 0, axial.r()}
	{ y() = -x() - z(); }

	Cubic::operator Axial() const
	{ return Axial{x(), z()}; }

	f32vec2 Axial::to_position(f32 radius) const
	{
		return {
				radius * (3.f / 2.f * f32(q())),
				radius * (sqrtf(3.f) / 2.f * f32(q()) + sqrtf(3.f) * f32(r()))
		};
	}

	Axial Axial::from_position(f32vec2 const& position, f32 radius)
	{
		f32 q = (2.f / 3.f * position.x()) / radius;
		f32 r = (-1.f / 3.f * position.x() + sqrtf(3.f) / 3.f * position.y()) / radius;

		f32vec3 cube{q, -q - r, r};
		f32vec3 diff{
				abs(roundf(cube.x()) - cube.x()),
				abs(roundf(cube.y()) - cube.y()),
				abs(roundf(cube.z()) - cube.z()),
		};

		cube = round(cube);
		if (diff.x() > diff.y() && diff.x() > diff.z())
			cube.x() = -cube.y() - cube.z();
		else if (diff.y() > diff.z())
			cube.y() = -cube.x() - cube.z();
		else
			cube.z() = -cube.x() - cube.y();

		return Axial{Cubic{
				static_cast<i32>(cube.x()),
				static_cast<i32>(cube.y()),
				static_cast<i32>(cube.z())
		}};
	}

	Tile::Tile(Axial const& Coord, u64 TypeIndex)
			: coord{Coord}, type_index{TypeIndex}
	{
		/*PluginManager::Manager<Trade::AbstractImporter> plugins;
		auto importer = plugins.loadAndInstantiate("StbImageImporter");
		if (!importer) Fatal{} << "Cannot load StbImageImporter plugin";

		string filename = detail::get_texture_filename(type);
		if (!importer->openFile(filename))
			Fatal{} << "Cannot load" << filename;

		auto image = importer->image2D(0);
		CORRADE_INTERNAL_ASSERT(image);

		texture.setWrapping(GL::SamplerWrapping::ClampToEdge)
				.setMagnificationFilter(GL::SamplerFilter::Nearest)
				.setMinificationFilter(GL::SamplerFilter::Linear)
				.setMaxLevel(4)
				.setStorage(1, GL::textureFormat(image->format()), image->size())
				.setSubImage(0, {}, *image)
				.generateMipmap();

		Trade::MeshData data = hexSolid(radius);
		mesh = MeshTools::compile(data, MeshTools::CompileFlag::GenerateFlatNormals);*/
	}

	void Grid::load_terrain_types()
	{
		string content;

		{
			std::ifstream file;
			file.open("assets/terrain.json");
			if (!file.is_open())
				throw std::runtime_error{"File not found"};

			auto begin = file.tellg();
			file.seekg(0, std::ios::end);
			auto end = file.tellg();
			file.seekg(0, std::ios::beg);

			content.resize(end - begin);
			file.read(content.data(), end - begin);
			file.close();
		}

		json data = json::parse(content);
		for (auto& type : data)
		{
			TerrainTypeInfo tmp{};
			tmp.name = type["name"].get<string>();
			tmp.texture = type["texture"].get<string>();
			tmp.index = _types.size();
			tmp.layer = nullopt;

			_types.push_back(std::move(tmp));
		}
	}

	void Grid::create_texture()
	{
		_texArray = GL::Texture2DArray{};
		_texArray.setWrapping(GL::SamplerWrapping::ClampToEdge)
				.setMagnificationFilter(GL::SamplerFilter::Nearest)
				.setMinificationFilter(GL::SamplerFilter::Linear)
				.setMaxLevel(4)
				.setStorage(1, GL::TextureFormat::RGBA8,
				            {512, 512, static_cast<i32>(_types.size())});

		PluginManager::Manager<Trade::AbstractImporter> plugins;
		auto importer = plugins.loadAndInstantiate("StbImageImporter");
		if (!importer) Fatal{} << "Cannot load StbImageImporter plugin";

		for (u64 i = 0; i < _types.size(); ++i)
		{
			if (!importer->openFile(_types[i].texture))
				Fatal{} << "Cannot load" << _types[i].texture;

			auto file = importer->image2D(0);
			CORRADE_INTERNAL_ASSERT(file);

			ImageView3D image{file->format(), {512, 512, 1}, file->data()};
			_texArray.setSubImage(0, i32vec3::zAxis(i), image);
			_types[i].layer = i;
		}

		_texArray.generateMipmap();
	}

	optional<reference_wrapper<Tile>> Grid::at(const Axial& coords)
	{
		if (_grid.find(coords) == _grid.end())
			return nullopt;

		return ref(_grid.at(coords));
	}

	void Grid::insert(Axial const& coords, u64 type_index)
	{
		_grid.insert_or_assign(coords, Tile{coords, type_index});
	}
}
