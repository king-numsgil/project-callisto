#pragma once

#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/TextureArray.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <unordered_map>

#include "Types.hpp"

namespace hex
{
	struct Axial;
	struct Cubic;

	struct Axial
	{
		Axial(Axial const&) noexcept = default;

		Axial& operator=(Axial const&) noexcept = default;

		Axial(Axial&&) noexcept = default;

		Axial& operator=(Axial&&) noexcept = default;

		inline Axial(i32 Q, i32 R) : data{Q, R}
		{}

		explicit Axial(Cubic const&);

		explicit operator Cubic() const;

		inline bool operator==(Axial const& other) const
		{ return data == other.data; }

		inline i32& q()
		{ return data.x(); }

		inline i32 q() const
		{ return data.x(); }

		inline i32& r()
		{ return data.y(); }

		inline i32 r() const
		{ return data.y(); }

		[[nodiscard]] f32vec2 to_position(f32 radius = 100.f) const;

		static Axial from_position(f32vec2 const& position, f32 radius = 100.f);

		i32vec2 data;
	};

	struct Cubic
	{
		Cubic(Cubic const&) noexcept = default;

		Cubic& operator=(Cubic const&) noexcept = default;

		Cubic(Cubic&&) noexcept = default;

		Cubic& operator=(Cubic&&) noexcept = default;

		inline Cubic(i32 X, i32 Y, i32 Z) : data{X, Y, Z}
		{}

		explicit Cubic(Axial const&);

		explicit operator Axial() const;

		inline bool operator==(Cubic const& other) const
		{ return data == other.data; }

		inline i32& x()
		{ return data.x(); }

		inline i32 x() const
		{ return data.x(); }

		inline i32& y()
		{ return data.y(); }

		inline i32 y() const
		{ return data.y(); }

		inline i32& z()
		{ return data.z(); }

		inline i32 z() const
		{ return data.z(); }

		i32vec3 data;
	};

	struct Tile
	{
		Tile(Tile const&) noexcept = delete;

		Tile& operator=(Tile const&) noexcept = delete;

		Tile(Tile&&) noexcept = default;

		Tile& operator=(Tile&&) noexcept = default;

		Tile(Axial const& Coord, u64 TypeIndex);

		Axial coord;
		u64 type_index;
	};

	inline Debug& operator<<(Debug& debug, Axial const& value)
	{
		return debug << "hex.Axial("
				<< Debug::nospace << value.q()
				<< Debug::nospace << ","
				<< Debug::nospace << value.r()
				<< Debug::nospace << ")";
	}

	inline Debug& operator<<(Debug& debug, Cubic const& value)
	{
		return debug << "hex.Cubic("
				<< Debug::nospace << value.x()
				<< Debug::nospace << ","
				<< Debug::nospace << value.y()
				<< Debug::nospace << ","
				<< Debug::nospace << value.z()
				<< Debug::nospace << ")";
	}
}

namespace detail
{
	template<class T>
	inline void hash_combine(std::size_t& seed, const T& v)
	{ seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2); }
}

template<>
struct std::hash<hex::Axial>
{
	hash() = default;

	std::size_t operator()(hex::Axial const& key) const
	{
		std::size_t hash = 0;
		detail::hash_combine(hash, key.q());
		detail::hash_combine(hash, key.r());
		return hash;
	}
};

template<>
struct std::hash<hex::Cubic>
{
	std::size_t operator()(hex::Cubic const& key) const
	{
		std::hash<hex::Axial> hasher;
		return hasher(hex::Axial{key});
	}
};

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
		Magnum::GL::Texture2DArray _texArray{NoCreate};
		vector<TerrainTypeInfo> _types{};
		std::unordered_map<Axial, Tile> _grid{};

		Magnum::GL::Buffer _pointBuffer{NoCreate};
		Magnum::GL::Mesh _mesh{NoCreate};
		HexShader _shader{NoCreate};
		bool _dirty{true};
	};
}
