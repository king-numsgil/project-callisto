#include <callisto/binding/angle.hpp>
#include <Corrade/Utility/Debug.h>
#include <Magnum/Math/Constants.h>
#include <Magnum/Math/Angle.h>
#include <type_traits>
#include <sstream>
#include <string>

using std::string;

using f32 = Magnum::Float;
using f64 = Magnum::Double;

namespace callisto::binding
{
	namespace detail
	{
		template<typename T>
		void register_constants(sol::state& _lua, string const& name)
		{
			using U = Magnum::Math::Constants<T>;

			sol::usertype<U> type = _lua.new_usertype<U>(name, sol::no_constructor);
			type["pi"] = &U::pi;
			type["piHalf"] = &U::piHalf;
			type["piQuarter"] = &U::piQuarter;
			type["tau"] = &U::tau;
			type["e"] = &U::e;
			type["sqrt2"] = &U::sqrt2;
			type["sqrt3"] = &U::sqrt3;
			type["sqrtHalf"] = &U::sqrtHalf;
			type["nan"] = &U::nan;
			type["inf"] = &U::inf;
		}

		template<typename T>
		void register_deg(sol::state& _lua, string const& name)
		{
			using U = Magnum::Math::Deg<T>;
			using V = Magnum::Math::Rad<T>;

			sol::constructors<U(), U(T), U(V)> ctor;
			sol::usertype<U> type = _lua.new_usertype<U>(name, ctor);

			type.set(sol::meta_function::unary_minus,
			         [](U self) -> U
			         { return -self; }
			);
			type.set(sol::meta_function::addition,
			         [](U self, U const& other) -> U
			         { return self + other; }
			);
			type.set(sol::meta_function::subtraction,
			         [](U self, U const& other) -> U
			         { return self - other; });
			type.set(sol::meta_function::multiplication,
			         [](U self, T scalar) -> U
			         { return self * scalar; }
			);
			type.set(sol::meta_function::division,
			         [](U self, T scalar) -> U
			         { return self / scalar; }
			);
			type.set(sol::meta_function::to_string,
			         [](U self) -> string
			         {
				         std::ostringstream str;
				         Corrade::Utility::Debug{&str, Corrade::Utility::Debug::Flag::NoNewlineAtTheEnd} << self;
				         return str.str();
			         }
			);
		}

		template<typename T>
		void register_rad(sol::state& _lua, string const& name)
		{
			using V = Magnum::Math::Deg<T>;
			using U = Magnum::Math::Rad<T>;

			sol::constructors<U(), U(T), U(V)> ctor;
			sol::usertype<U> type = _lua.new_usertype<U>(name, ctor);

			type.set(sol::meta_function::unary_minus,
			         [](U self) -> U
			         { return -self; }
			);
			type.set(sol::meta_function::addition,
			         [](U self, U const& other) -> U
			         { return self + other; }
			);
			type.set(sol::meta_function::subtraction,
			         [](U self, U const& other) -> U
			         { return self - other; }
			);
			type.set(sol::meta_function::multiplication,
			         [](U self, T scalar) -> U
			         { return self * scalar; }
			);
			type.set(sol::meta_function::division,
			         [](U self, T scalar) -> U
			         { return self / scalar; }
			);
			type.set(sol::meta_function::to_string,
			         [](U self) -> string
			         {
				         std::ostringstream str;
				         Corrade::Utility::Debug{&str} << Corrade::Utility::Debug::Flag::NoNewlineAtTheEnd << self;
				         return str.str();
			         }
			);
		}
	}

	void register_angle(sol::state& _lua)
	{
		detail::register_constants<f32>(_lua, "f32const");
		detail::register_constants<f64>(_lua, "f64const");

		detail::register_deg<f32>(_lua, "f32deg");
		detail::register_deg<f64>(_lua, "f64deg");

		detail::register_rad<f32>(_lua, "f32rad");
		detail::register_rad<f64>(_lua, "f64rad");
	}
}
