#include <callisto/binding/vec2.hpp>
#include <Corrade/Utility/Debug.h>
#include <Magnum/Math/Vector2.h>
#include <type_traits>
#include <sstream>
#include <string>

using std::string;

using i8 = int8_t;
using u8 = uint8_t;

using i16 = int16_t;
using u16 = uint16_t;

using i32 = int32_t;
using u32 = uint32_t;

using i64 = int64_t;
using u64 = uint64_t;

using f32 = Magnum::Float;
using f64 = Magnum::Double;

template<class T> using vec2 = Magnum::Math::Vector2<T>;

namespace callisto::binding
{
	namespace detail
	{
		template<typename T>
		void register_vec2(sol::state& _lua, string const& name)
		{
			using U = vec2<T>;
			sol::constructors<U(), U(T), U(T, T)> ctor;
			sol::usertype<U> type = _lua.new_usertype<U>(name, ctor);

			type["x"] = (T& (U::*)()) &U::x;
			type["y"] = (T& (U::*)()) &U::y;

			if constexpr (std::is_signed<T>::value)
			{
				type.set(sol::meta_function::unary_minus, (U(U::*)() const) &U::operator-);

				type["perpendicular"] = &U::template perpendicular<T>;
			}

			if constexpr (std::is_floating_point<T>::value)
			{
				type["aspectRatio"] = &U::template aspectRatio<T>;
				type["lengthInverted"] = &U::template lengthInverted<T>;
				type["normalized"] = &U::template normalized<T>;
				type["resized"] = &U::template resized<T>;
				type["projected"] = &U::template projected<T>;
				type["projectedOntoNormalized"] = &U::template projectedOntoNormalized<T>;
			}

			type.set(sol::meta_function::addition, &U::operator+);
			type.set(sol::meta_function::subtraction,
			         [](U self, U const& other) -> U
			         { return self - other; });
			type.set(sol::meta_function::multiplication, sol::overload(
					[](U self, U const& other) -> U
					{ return self * other; },
					[](U self, T scalar) -> U
					{ return self * scalar; }
			));
			type.set(sol::meta_function::division, sol::overload(
					[](U self, U const& other) -> U
					{ return self / other; },
					[](U self, T scalar) -> U
					{ return self / scalar; }
			));
			type.set(sol::meta_function::to_string,
			         [](U self) -> string
			         {
				         std::ostringstream str;
				         Corrade::Utility::Debug{&str, Corrade::Utility::Debug::Flag::NoNewlineAtTheEnd} << self;
				         return str.str();
			         }
			);

			type["dot"] = &U::dot;
			type["length"] = &U::length;
			type["flipped"] = &U::flipped;
			type["sum"] = &U::sum;
			type["product"] = &U::product;
			type["min"] = &U::min;
			type["max"] = &U::max;
			type["minmax"] = &U::minmax;
		}
	}

	void register_vec2(sol::state& _lua)
	{
		detail::register_vec2<i8>(_lua, "i8vec2");
		detail::register_vec2<u8>(_lua, "u8vec2");
		detail::register_vec2<i16>(_lua, "i16vec2");
		detail::register_vec2<u16>(_lua, "u16vec2");
		detail::register_vec2<i32>(_lua, "i32vec2");
		detail::register_vec2<u32>(_lua, "u32vec2");
		detail::register_vec2<i64>(_lua, "i64vec2");
		detail::register_vec2<u64>(_lua, "u64vec2");
		detail::register_vec2<f32>(_lua, "f32vec2");
		detail::register_vec2<f64>(_lua, "f64vec2");
	}
}
