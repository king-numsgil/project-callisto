#pragma once

#include "../Types.hpp"

namespace hex
{
	constexpr bool FLAT_TOPPED = false;
	constexpr f32 RADIUS = 50.f;

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

		[[nodiscard]] f32vec2 to_position() const;

		static Axial from_position(f32vec2 const& position);

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

template<>
struct std::hash<hex::Axial>
{
	hash() = default;

	std::size_t operator()(hex::Axial const& key) const
	{
		std::size_t hash = 0;
		hash_combine(hash, key.q());
		hash_combine(hash, key.r());
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
