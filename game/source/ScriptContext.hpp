#pragma once

#include <sol/state.hpp>

#include "Types.hpp"

class ScriptContext
{
public:
	ScriptContext();

	~ScriptContext() = default;

	ScriptContext(ScriptContext const&) = delete;

	ScriptContext& operator=(ScriptContext const&) = delete;

	ScriptContext(ScriptContext&&) = default;

	ScriptContext& operator=(ScriptContext&&) = default;

	void eval(string const& code);

	void file(string const& filename);

protected:
	void register_math_types();

	sol::state _lua;
};
