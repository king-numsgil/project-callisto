#include <Magnum/Trade/AbstractImporter.h>
#include <Corrade/Containers/Reference.h>
#include <Corrade/Containers/Optional.h>
#include <Magnum/MeshTools/Compile.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/GL/Version.h>
#include <Magnum/GL/Shader.h>
#include <Magnum/ImageView.h>
#include <nlohmann/json.hpp>
#include <fstream>

#include "Grid.hpp"

using namespace Magnum;

using json = nlohmann::json;

struct HexPointData
{
	f32vec2 position{};
	i32 layer{-1};
};

namespace hex
{
	Grid::HexShader::HexShader()
	{
		Utility::Resource rs("CallistoShaders");

		GL::Shader vert{GL::Version::GL430, GL::Shader::Type::Vertex};
		vert.addSource(rs.get("hex/vert.glsl"));

		GL::Shader geo{GL::Version::GL430, GL::Shader::Type::Geometry};
		geo.addSource(rs.get("hex/geo.glsl"));

		GL::Shader frag{GL::Version::GL430, GL::Shader::Type::Fragment};
		frag.addSource(rs.get("hex/frag.glsl"));

		CORRADE_INTERNAL_ASSERT_OUTPUT(GL::Shader::compile({vert, geo, frag}));

		attachShaders({vert, geo, frag});

		CORRADE_INTERNAL_ASSERT_OUTPUT(link());

		_flatToppedLocation = uniformLocation("flat_topped");
		_pickCoordLocation = uniformLocation("pick_coord");
		_transformLocation = uniformLocation("transform");
		_radiusLocation = uniformLocation("radius");
	}

	Grid::HexShader& Grid::HexShader::set_flat_topped(bool value)
	{
		setUniform(_flatToppedLocation, value ? i32(1) : i32(0));
		return *this;
	}

	Grid::HexShader& Grid::HexShader::set_transformation_matrix(f32mat3 const& matrix)
	{
		setUniform(_transformLocation, matrix);
		return *this;
	}

	Grid::HexShader& Grid::HexShader::set_radius(f32 radius)
	{
		setUniform(_radiusLocation, radius);
		return *this;
	}

	Grid::HexShader& Grid::HexShader::set_pick_location(Axial const& coord)
	{
		setUniform(_pickCoordLocation, f32vec2{coord.data});
		return *this;
	}

	Grid::HexShader& Grid::HexShader::bind_texture(GL::Texture2DArray& texture)
	{
		texture.bind(0);
		return *this;
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
			tmp.layer = -1;

			_types.push_back(std::move(tmp));
		}
	}

	void Grid::create_texture()
	{
		_texArray = GL::Texture2DArray{};
		_texArray.setWrapping(GL::SamplerWrapping::ClampToEdge)
				.setMagnificationFilter(GL::SamplerFilter::Linear)
				.setMinificationFilter(GL::SamplerFilter::Linear)
				.setMaxLevel(4)
				.setStorage(1, GL::TextureFormat::RGBA8,
				            {140, 140, static_cast<i32>(_types.size())});

		PluginManager::Manager<Trade::AbstractImporter> plugins;
		auto importer = plugins.loadAndInstantiate("StbImageImporter");
		if (!importer) Fatal{} << "Cannot load StbImageImporter plugin";

		for (u64 i = 0; i < _types.size(); ++i)
		{
			if (!importer->openFile(_types[i].texture))
				Fatal{} << "Cannot load" << _types[i].texture;

			auto file = importer->image2D(0);
			CORRADE_INTERNAL_ASSERT(file);

			ImageView3D image{file->format(), {140, 140, 1}, file->data()};
			_texArray.setSubImage(0, i32vec3::zAxis(i), image);
			_types[i].layer = (i32) i;
		}

		_texArray.generateMipmap();
	}

	optional<reference_wrapper<Tile>> Grid::at(const Axial& coords)
	{
		if (!_grid.contains(coords))
			return nullopt;

		return ref(_grid.at(coords));
	}

	void Grid::insert(Axial const& coords, u64 type_index)
	{
		_grid.insert_or_assign(coords, Tile{coords, type_index});
		_dirty = true;
	}

	void Grid::render(f32mat3 const& transform)
	{
		ensure_created();

		if (_dirty)
		{
			build_hex_mesh();
			_dirty = false;
		}

		render_hex_mesh(transform);
	}

	void Grid::ensure_created()
	{
		if (_hexShader.id() == 0)
		{
			_hexShader = HexShader{};
			_hexShader.set_flat_topped(FLAT_TOPPED)
					.set_radius(RADIUS);
		}

		if (_hexMesh.id() == 0)
		{
			_pointBuffer = GL::Buffer{};

			_hexMesh = GL::Mesh{};
			_hexMesh.setPrimitive(GL::MeshPrimitive::Points)
					.setCount(_grid.size())
					.addVertexBuffer(_pointBuffer, 0, HexShader::Coords{}, HexShader::Layer{});
		}
	}

	void Grid::render_hex_mesh(const f32mat3& transform)
	{
		_hexShader.set_transformation_matrix(transform)
				.set_pick_location(_pickCoord)
				.bind_texture(_texArray)
				.draw(_hexMesh);
	}

	void Grid::build_hex_mesh()
	{
		Containers::Array<HexPointData> points{Containers::DefaultInit, _grid.size()};
		u64 index = 0;

		for (auto& tile : _grid)
		{
			points[index].layer = _types[tile.second.type_index].layer;
			points[index].position = f32vec2{(f32) tile.second.coord.q(), (f32) tile.second.coord.r()};
			index++;
		}

		_pointBuffer.setData(points);
	}
}
