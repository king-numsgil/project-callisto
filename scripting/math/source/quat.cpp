#include <callisto/binding/quat.hpp>
#include <Magnum/Math/DualQuaternion.h>
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
		void register_quat(sol::state& _lua, string const& name)
		{
			using vec = Magnum::Math::Vector3<T>;
			using U = Magnum::Math::Quaternion<T>;
			sol::constructors<U(), U(vec const&)> ctor;
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
					{ return self * scalar; }
			));
			type.set(sol::meta_function::division, sol::overload(
					[](T scalar, U const& other) -> decltype(T{} / U{})
					{ return scalar / other; },
					[](U self, T scalar) -> decltype(U{} / T{})
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

			type["rotation"] = &U::rotation;
			type["vector"] = (vec& (U::*)()) &U::vector;
			type["scalar"] = (T& (U::*)()) &U::scalar;
			type["toEuler"] = &U::toEuler;
			type["angle"] = &U::angle;
			type["dot"] = &U::dot;
			type["length"] = &U::length;
			type["normalized"] = &U::normalized;
			type["conjugated"] = &U::conjugated;
			type["inverted"] = &U::inverted;
			type["invertedNormalized"] = &U::invertedNormalized;
			type["transformVector"] = &U::transformVector;
			type["transformVectorNormalized"] = &U::transformVectorNormalized;
		}

		template<typename T>
		void register_dquat(sol::state& _lua, string const& name)
		{
			using vec = Magnum::Math::Vector3<T>;
			using quat = Magnum::Math::Quaternion<T>;
			using U = Magnum::Math::DualQuaternion<T>;
			sol::constructors<U(), U(quat const&, quat const&), U(vec const&)> ctor;
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
					(U(*)(Magnum::Math::Rad<T>, vec const&)) &U::rotation,
					(quat(U::*)() const) &U::rotation
			);
			type["translation"] = sol::overload(
					(U(*)(vec const&)) &U::translation,
					(vec(U::*)() const) &U::translation
			);
			type["real"] = (quat& (U::*)()) &U::real;
			type["dual"] = (quat& (U::*)()) &U::dual;

			type["isNormalized"] = &U::isNormalized;
			type["dualConjugated"] = &U::dualConjugated;
			type["conjugated"] = &U::conjugated;
			type["lengthSquared"] = &U::lengthSquared;
			type["length"] = &U::length;
			type["normalized"] = &U::normalized;
			type["inverted"] = &U::inverted;
			type["invertedNormalized"] = &U::invertedNormalized;
			type["transformVector"] = &U::transformVector;
			type["transformVectorNormalized"] = &U::transformVectorNormalized;
			type["transformPoint"] = &U::transformPoint;
			type["transformPointNormalized"] = &U::transformPointNormalized;
		}
	}

	void register_quaternion(sol::state& _lua)
	{
		detail::register_quat<f32>(_lua, "f32quat");
		detail::register_quat<f64>(_lua, "f64quat");

		detail::register_dquat<f32>(_lua, "f32dquat");
		detail::register_dquat<f64>(_lua, "f64dquat");
	}
}
