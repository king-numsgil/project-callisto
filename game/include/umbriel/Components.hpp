#pragma once

#include <Magnum/GL/Texture.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <box2d/b2_body.h>

#include <umbriel/Types.hpp>

namespace umbriel
{
	struct BodyComponent
	{
		b2Body* _body{nullptr};
	};

	struct SpriteComponent
	{
		Magnum::GL::Texture2D _texture{Magnum::NoCreate};
		Magnum::GL::Buffer _buffer{Magnum::NoCreate};
		Magnum::GL::Mesh _mesh{Magnum::NoCreate};

		void create(string const& filename, f32vec2 const& size);
	};
}
