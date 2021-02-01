#pragma once

#include "Types.hpp"

#include <entt/entity/handle.hpp>

struct TransformComponent
{
	f64dquat transform{Magnum::Math::IdentityInit};
	entt::const_handle parent;

	explicit TransformComponent(entt::registry const& registry) : parent{registry, entt::null}
	{}

	[[nodiscard]] f64dquat world_transform() const
	{
		if (parent)
			return parent.get<TransformComponent>().transform * transform;
		else
			return transform;
	}
};
