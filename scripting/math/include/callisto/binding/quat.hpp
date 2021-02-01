#pragma once

#include <ScriptingMath_export.h>
#include <sol/sol.hpp>

namespace callisto::binding
{
	SCRIPTING_API void register_quaternion(sol::state& _lua);
}
