#pragma once

#include <Magnum/GL/Texture.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>

#include <umbriel/physics/Shape.hpp>
#include <umbriel/physics/Body.hpp>
#include <umbriel/Types.hpp>

#include <memory>
#include <vector>

namespace umbriel
{
	struct BodyComponent
	{
		physics::Body _body{};
	};

	struct SpriteComponent
	{
		Magnum::GL::Texture2D _texture{Magnum::NoCreate};
		Magnum::GL::Buffer _buffer{Magnum::NoCreate};
		Magnum::GL::Mesh _mesh{Magnum::NoCreate};

		void create(string const& filename, f32vec2 const& size);
	};
}
