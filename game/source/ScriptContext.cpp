#include <sol/sol.hpp>

#include "ScriptContext.hpp"

using Corrade::Utility::Error;

ScriptContext::ScriptContext()
		: _lua{}
{
	_lua.open_libraries();

	register_math_types();
}

void ScriptContext::eval(string const& code)
{
	try
	{
		_lua.safe_script(code);
	}
	catch(const sol::error& e)
	{
		Error{} << "Script error" << e.what();
	}
}

void ScriptContext::file(string const& filename)
{
	try
	{
		_lua.safe_script_file(filename);
	}
	catch(const sol::error& e)
	{
		Error{} << "Script error" << e.what();
	}
}

#include <callisto/binding-math.hpp>

void ScriptContext::register_math_types()
{
	using namespace callisto::binding;

	register_vec2(_lua);
	register_vec3(_lua);
	register_vec4(_lua);
	register_mat(_lua);
	register_angle(_lua);
	register_complex(_lua);
	register_quaternion(_lua);
}
