#pragma once

#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/DimensionTraits.h>
#include <Magnum/Shaders/Generic.h>
#include <umbriel/Types.hpp>

namespace umbriel
{
	namespace detail
	{
		enum class FlatFlag : u8
		{
			Textured = 1u << 0u,
			AlphaMask = 1u << 1u,
			VertexColor = 1u << 2u,
			ObjectId = 1u << 3u
		};
		typedef Corrade::Containers::EnumSet<FlatFlag> FlatFlags;
	}

	class FlatShader : public Magnum::GL::AbstractShaderProgram
	{
	public:
		typedef typename Magnum::Shaders::Generic<2>::Position Position;
		typedef typename Magnum::Shaders::Generic<2>::TextureCoordinates TextureCoordinates;
		typedef typename Magnum::Shaders::Generic<2>::Color3 Color3;
		typedef typename Magnum::Shaders::Generic<2>::Color4 Color4;

		enum : u8
		{
			ColorOutput = Magnum::Shaders::Generic<2>::ColorOutput,
			ObjectIdOutput = Magnum::Shaders::Generic<2>::ObjectIdOutput
		};

		typedef detail::FlatFlag Flag;
		typedef detail::FlatFlags Flags;

		explicit FlatShader(Flags flags = {});

		explicit FlatShader(Magnum::NoCreateT) noexcept: Magnum::GL::AbstractShaderProgram{Magnum::NoCreate}
		{}

		FlatShader(FlatShader const&) = delete;

		FlatShader& operator=(FlatShader const&) = delete;

		FlatShader(FlatShader&&) noexcept = default;

		FlatShader& operator=(FlatShader&&) noexcept = default;

		[[nodiscard]]
		Flags flags() const
		{ return _flags; }

		FlatShader& set_transformation_projection_matrix(f64mat3 const& matrix);

		FlatShader& set_color(f32col4 const& color);

		FlatShader& bind_texture(Magnum::GL::Texture2D& texture);

		FlatShader& set_alpha_mask(f32 mask);

		FlatShader& set_object_id(u32 id);

	private:
		Flags _flags;
		i32 _transformationProjectionMatrixUniform{0},
				_colorUniform{1},
				_alphaMaskUniform{2},
				_objectIdUniform{3};
	};
}
