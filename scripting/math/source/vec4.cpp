#include <callisto/binding/vec4.hpp>
#include <Corrade/Utility/Debug.h>
#include <Magnum/Math/Vector4.h>
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

template<class T> using vec3 = Magnum::Math::Vector3<T>;
template<class T> using vec4 = Magnum::Math::Vector4<T>;

namespace callisto::binding
{
	namespace detail
	{
		template<typename T>
		void register_vec4(sol::state& _lua, string const& name)
		{
			using U = vec4<T>;
			sol::constructors<U(), U(T), U(T, T, T, T), U(vec3<T>, T)> ctor;
			sol::usertype<U> type = _lua.new_usertype<U>(name, ctor);

			type["x"] = (T& (U::*)()) &U::x;
			type["y"] = (T& (U::*)()) &U::y;
			type["z"] = (T& (U::*)()) &U::z;
			type["w"] = (T& (U::*)()) &U::w;

			if constexpr (std::is_signed<T>::value)
			{
				type.set(sol::meta_function::unary_minus, (U(U::*)() const) &U::operator-);
			}

			if constexpr (std::is_floating_point<T>::value)
			{
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

	void register_vec4(sol::state& _lua)
	{
		detail::register_vec4<i8>(_lua, "i8vec4");
		detail::register_vec4<u8>(_lua, "u8vec4");
		detail::register_vec4<i16>(_lua, "i16vec4");
		detail::register_vec4<u16>(_lua, "u16vec4");
		detail::register_vec4<i32>(_lua, "i32vec4");
		detail::register_vec4<u32>(_lua, "u32vec4");
		detail::register_vec4<i64>(_lua, "i64vec4");
		detail::register_vec4<u64>(_lua, "u64vec4");
		detail::register_vec4<f32>(_lua, "f32vec4");
		detail::register_vec4<f64>(_lua, "f64vec4");
	}
}
