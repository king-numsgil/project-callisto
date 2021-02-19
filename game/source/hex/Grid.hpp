#pragma once

#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/TextureArray.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <unordered_map>

#include "Coords.hpp"
#include "Tile.hpp"

namespace hex
{
	struct TerrainTypeInfo
	{
		string name, texture;
		u64 index;
		i32 layer;
	};

	class Grid
	{
	public:
		class HexShader : public Magnum::GL::AbstractShaderProgram
		{
		public:
			using Coords = Magnum::GL::Attribute<0, f32vec2>;
			using Layer = Magnum::GL::Attribute<1, i32>;

			explicit HexShader();

			explicit HexShader(NoCreateT) noexcept: Magnum::GL::AbstractShaderProgram{NoCreate}
			{}

			HexShader(HexShader const&) noexcept = delete;

			HexShader& operator=(HexShader const&) noexcept = delete;

			HexShader(HexShader&&) noexcept = default;

			HexShader& operator=(HexShader&&) noexcept = default;

			HexShader& set_flat_topped(bool value);

			HexShader& set_transformation_matrix(f32mat3 const& matrix);

			HexShader& set_radius(f32 radius = 100.f);

			HexShader& bind_texture(Magnum::GL::Texture2DArray& texture);

		private:
			using Magnum::GL::AbstractShaderProgram::drawTransformFeedback;
			using Magnum::GL::AbstractShaderProgram::dispatchCompute;

			i32 _transformLocation{0}, _radiusLocation{1}, _flatToppedLocation{2};
		};

		Grid() = default;

		~Grid() = default;

		Grid(Grid const&) noexcept = delete;

		Grid& operator=(Grid const&) noexcept = delete;

		Grid(Grid&&) noexcept = default;

		Grid& operator=(Grid&&) noexcept = default;

		Magnum::GL::Texture2DArray& texture_array()
		{ return _texArray; }

		vector<TerrainTypeInfo>& terrain_types()
		{ return _types; }

		u64 size() const
		{ return _grid.size(); }

		void load_terrain_types();

		void create_texture();

		void set_dirty(bool value)
		{ _dirty = value; }

		bool is_dirty() const
		{ return _dirty; }

		optional<reference_wrapper<Tile>> at(Axial const& coords);

		void insert(Axial const& coords, u64 type_index);

		void render(f32mat3 const& transform);

	private:
		void ensure_created();
		void render_hex_mesh(f32mat3 const& transform);
		void build_hex_mesh();

		Magnum::GL::Texture2DArray _texArray{NoCreate};
		vector<TerrainTypeInfo> _types{};
		std::unordered_map<Axial, Tile> _grid{};

		Magnum::GL::Buffer _pointBuffer{NoCreate};
		Magnum::GL::Mesh _hexMesh{NoCreate};
		HexShader _hexShader{NoCreate};
		bool _dirty{true};
	};
}
