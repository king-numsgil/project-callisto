#include <callisto/binding/complex.hpp>
#include <Magnum/Math/DualComplex.h>
#include <Corrade/Utility/Debug.h>
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
		void register_comp(sol::state& _lua, string const& name)
		{
			using vec = Magnum::Math::Vector2<T>;
			using U = Magnum::Math::Complex<T>;
			sol::constructors<U(), U(T, T), U(vec const&)> ctor;
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
			type.set(sol::meta_function::multiplication, sol::overload(
					[](U self, U const& other) -> decltype(U{} * U{})
					{ return self * other; },
					[](T scalar, U const& other) -> decltype(T{} * U{})
					{ return scalar * other; },
					[](U self, T scalar) -> decltype(U{} * T{})
					{ return self * scalar; },
					[](vec v, U const& other) -> decltype(vec{} * U{})
					{ return v * other; },
					[](U self, vec v) -> decltype(U{} * vec{})
					{ return self * v; }
			));
			type.set(sol::meta_function::division, sol::overload(
					[](T scalar, U const& other) -> decltype(T{} / U{})
					{ return scalar / other; },
					[](U self, T scalar) -> decltype(U{} / T{})
					{ return self / scalar; },
					[](vec v, U const& other) -> decltype(vec{} / U{})
					{ return v / other; },
					[](U self, vec v) -> decltype(U{} / vec{})
					{ return self / v; }
			));
			type.set(sol::meta_function::to_string,
			         [](U self) -> string
			         {
				         std::ostringstream str;
				         Corrade::Utility::Debug{&str, Corrade::Utility::Debug::Flag::NoNewlineAtTheEnd} << self;
				         return str.str();
			         }
			);

			type["rotation"] = &U::rotation;
			type["real"] = (T& (U::*)()) &U::real;
			type["imaginary"] = (T& (U::*)()) &U::imaginary;
			type["angle"] = &U::angle;
			type["dot"] = &U::dot;
			type["length"] = &U::length;
			type["normalized"] = &U::normalized;
			type["conjugated"] = &U::conjugated;
			type["inverted"] = &U::inverted;
			type["invertedNormalized"] = &U::invertedNormalized;
			type["transformVector"] = &U::transformVector;
		}

		template<typename T>
		void register_dcomp(sol::state& _lua, string const& name)
		{
			using vec = Magnum::Math::Vector2<T>;
			using comp = Magnum::Math::Complex<T>;
			using U = Magnum::Math::DualComplex<T>;
			sol::constructors<U(), U(comp const&, comp const&), U(vec const&)> ctor;
			sol::usertype<U> type = _lua.new_usertype<U>(name, ctor);

			type.set(sol::meta_function::multiplication,
			         [](U self, U const& other) -> U
			         { return self * other; }
			);
			type.set(sol::meta_function::to_string,
			         [](U self) -> string
			         {
				         std::ostringstream str;
				         Corrade::Utility::Debug{&str, Corrade::Utility::Debug::Flag::NoNewlineAtTheEnd} << self;
				         return str.str();
			         }
			);

			type["rotation"] = sol::overload(
					(U(*)(Magnum::Math::Rad<T>)) &U::rotation,
					(comp(U::*)() const) &U::rotation
			);
			type["translation"] = sol::overload(
					(U(*)(vec const&)) &U::translation,
					(vec(U::*)() const) &U::translation
			);
			type["real"] = (comp& (U::*)()) &U::real;
			type["dual"] = (comp& (U::*)()) &U::dual;

			type["isNormalized"] = &U::isNormalized;
			type["complexConjugated"] = &U::complexConjugated;
			type["dualConjugated"] = &U::dualConjugated;
			type["conjugated"] = &U::conjugated;
			type["lengthSquared"] = &U::lengthSquared;
			type["length"] = &U::length;
			type["normalized"] = &U::normalized;
			type["inverted"] = &U::inverted;
			type["invertedNormalized"] = &U::invertedNormalized;
			type["transformVector"] = &U::transformVector;
			type["transformPoint"] = &U::transformPoint;
		}
	}

	void register_complex(sol::state& _lua)
	{
		detail::register_comp<f32>(_lua, "f32comp");
		detail::register_comp<f64>(_lua, "f64comp");

		detail::register_dcomp<f32>(_lua, "f32dcomp");
		detail::register_dcomp<f64>(_lua, "f64dcomp");
	}
}
