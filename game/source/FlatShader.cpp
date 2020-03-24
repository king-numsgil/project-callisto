#include <Magnum/Shaders/Implementation/CreateCompatibilityShader.h>
#include <Corrade/Containers/EnumSet.hpp>
#include <Corrade/Containers/Reference.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/GL/Extensions.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/Shader.h>

#include <umbriel/FlatShader.hpp>

using namespace Corrade;
using namespace Magnum;

namespace
{
	enum: Int { TextureLayer = 0 };
}

namespace umbriel
{
	FlatShader::FlatShader(Flags flags) : _flags{flags}
	{
		Utility::Resource rs("UmbrielShaders");
		const GL::Version version = GL::Context::current().supportedVersion({GL::Version::GL400});

		GL::Shader vert = Shaders::Implementation::createCompatibilityShader(rs, version, GL::Shader::Type::Vertex);
		GL::Shader frag = Shaders::Implementation::createCompatibilityShader(rs, version, GL::Shader::Type::Fragment);

		vert.addSource(flags & Flag::Textured ? "#define TEXTURED\n" : "")
				.addSource(flags & Flag::VertexColor ? "#define VERTEX_COLOR\n" : "")
				.addSource(rs.get("generic.glsl"))
				.addSource(rs.get("Flat.vert"));
		frag.addSource(flags & Flag::Textured ? "#define TEXTURED\n" : "")
				.addSource(flags & Flag::AlphaMask ? "#define ALPHA_MASK\n" : "")
				.addSource(flags & Flag::VertexColor ? "#define VERTEX_COLOR\n" : "")
				.addSource(flags & Flag::ObjectId ? "#define OBJECT_ID\n" : "")
				.addSource(rs.get("generic.glsl"))
				.addSource(rs.get("Flat.frag"));

		CORRADE_INTERNAL_ASSERT_OUTPUT(GL::Shader::compile({vert, frag}));
		attachShaders({vert, frag});

		if(!GL::Context::current().isExtensionSupported<GL::Extensions::ARB::explicit_attrib_location>(version))
		{
			bindAttributeLocation(Position::Location, "position");
			if(flags & Flag::Textured)
				bindAttributeLocation(TextureCoordinates::Location, "textureCoordinates");
			if(flags & Flag::VertexColor)
				bindAttributeLocation(Color3::Location, "vertexColor"); /* Color4 is the same */
			if(flags & Flag::ObjectId) {
				bindFragmentDataLocation(ColorOutput, "color");
				bindFragmentDataLocation(ObjectIdOutput, "objectId");
			}
		}

		CORRADE_INTERNAL_ASSERT_OUTPUT(link());
		if(!GL::Context::current().isExtensionSupported<GL::Extensions::ARB::explicit_uniform_location>(version))
		{
			_transformationProjectionMatrixUniform = uniformLocation("transformationProjectionMatrix");
			_colorUniform = uniformLocation("color");
			if(flags & Flag::AlphaMask) _alphaMaskUniform = uniformLocation("alphaMask");
			if(flags & Flag::ObjectId) _objectIdUniform = uniformLocation("objectId");
		}

		if(!GL::Context::current().isExtensionSupported<GL::Extensions::ARB::shading_language_420pack>(version))
		{
			if(flags & Flag::Textured) setUniform(uniformLocation("textureData"), TextureLayer);
		}
	}

	FlatShader& FlatShader::set_transformation_projection_matrix(f64mat3 const& matrix)
	{
		setUniform(_transformationProjectionMatrixUniform, matrix);
		return *this;
	}

	FlatShader& FlatShader::set_color(f32col4 const& color)
	{
		setUniform(_colorUniform, color);
		return *this;
	}

	FlatShader& FlatShader::bind_texture(Magnum::GL::Texture2D& texture)
	{
		CORRADE_ASSERT(_flags & Flag::Textured,
		               "FlatShader::bind_texture(): the shader was not created with texturing enabled", *this);
		texture.bind(TextureLayer);
		return *this;
	}

	FlatShader& FlatShader::set_alpha_mask(f32 mask)
	{
		CORRADE_ASSERT(_flags & Flag::AlphaMask,
		               "FlatShader::set_alpha_mask(): the shader was not created with alpha mask enabled", *this);
		setUniform(_alphaMaskUniform, mask);
		return *this;
	}

	FlatShader& FlatShader::set_object_id(u32 id)
	{
		CORRADE_ASSERT(_flags & Flag::ObjectId,
		               "FlatShader::set_object_id(): the shader was not created with object ID enabled", *this);
		setUniform(_objectIdUniform, id);
		return *this;
	}
}
