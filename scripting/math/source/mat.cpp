#include <callisto/binding/mat.hpp>
#include <Corrade/Utility/Debug.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Math/Matrix4.h>
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
		void register_mat3(sol::state& _lua, string const& name)
		{
			using U = Magnum::Math::Matrix3<T>;
			using vec = Magnum::Math::Vector3<T>;

			sol::constructors<U(), U(T), U(vec const&, vec const&, vec const&)> ctor;
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

			type["translation"] = sol::overload(
					(U(*)(Magnum::Math::Vector2<T> const&)) &U::translation,
					(Magnum::Math::Vector2<T>& (U::*)()) &U::translation
			);
			type["scaling"] = (U(*)(Magnum::Math::Vector2<T> const&)) &U::scaling;
			type["rotation"] = (U(*)(Magnum::Math::Rad<T>)) &U::rotation;
			type["reflection"] = (U(*)(Magnum::Math::Vector2<T> const&)) &U::reflection;
			type["shearingX"] = (U(*)(T)) &U::shearingX;
			type["shearingY"] = (U(*)(T)) &U::shearingY;
			type["projection"] = (U(*)(Magnum::Math::Vector2<T> const&)) &U::projection;

			type["isRigidTransformation"] = &U::isRigidTransformation;
			type["right"] = (Magnum::Math::Vector2<T>& (U::*)()) &U::right;
			type["up"] = (Magnum::Math::Vector2<T>& (U::*)()) &U::up;
			type["inverted"] = &U::inverted;
			type["invertedOrthogonal"] = &U::invertedOrthogonal;
			type["invertedRigid"] = &U::invertedRigid;
			type["transformVector"] = &U::transformVector;
			type["transformPoint"] = &U::transformPoint;
		}

		template<typename T>
		void register_mat4(sol::state& _lua, string const& name)
		{
			using U = Magnum::Math::Matrix4<T>;
			using vec = Magnum::Math::Vector4<T>;

			sol::constructors<U(), U(T), U(vec const&, vec const&, vec const&, vec const&)> ctor;
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

			type["translation"] = sol::overload(
					(U(*)(Magnum::Math::Vector3<T> const&)) &U::translation,
					(Magnum::Math::Vector3<T>& (U::*)()) &U::translation
			);
			type["scaling"] = (U(*)(Magnum::Math::Vector3<T> const&)) &U::scaling;
			type["rotation"] = (U(*)(Magnum::Math::Rad<T>, Magnum::Math::Vector3<T> const&)) &U::rotation;
			type["rotationX"] = (U(*)(Magnum::Math::Rad<T>)) &U::rotationX;
			type["rotationY"] = (U(*)(Magnum::Math::Rad<T>)) &U::rotationY;
			type["rotationZ"] = (U(*)(Magnum::Math::Rad<T>)) &U::rotationZ;
			type["reflection"] = (U(*)(Magnum::Math::Vector3<T> const&)) &U::reflection;
			type["shearingXY"] = (U(*)(T, T)) &U::shearingXY;
			type["shearingXZ"] = (U(*)(T, T)) &U::shearingXZ;
			type["shearingYZ"] = (U(*)(T, T)) &U::shearingYZ;
			type["orthographicProjection"] = (U(*)(Magnum::Math::Vector2<T> const&, T, T)) &U::orthographicProjection;
			type["perspectiveProjection"] = sol::overload(
					(U(*)(Magnum::Math::Vector2<T> const&, T, T)) &U::perspectiveProjection,
					(U(*)(Magnum::Math::Rad<T>, T, T, T)) &U::perspectiveProjection,
					(U(*)(Magnum::Math::Vector2<T> const&, Magnum::Math::Vector2<T> const&, T,
					      T)) &U::perspectiveProjection
			);
			type["lookAt"] = &U::lookAt;

			type["isRigidTransformation"] = &U::isRigidTransformation;
			type["right"] = (Magnum::Math::Vector3<T>& (U::*)()) &U::right;
			type["up"] = (Magnum::Math::Vector3<T>& (U::*)()) &U::up;
			type["backward"] = (Magnum::Math::Vector3<T>& (U::*)()) &U::backward;
			type["inverted"] = &U::inverted;
			type["invertedOrthogonal"] = &U::invertedOrthogonal;
			type["invertedRigid"] = &U::invertedRigid;
			type["normalMatrix"] = &U::normalMatrix;
			type["transformVector"] = &U::transformVector;
			type["transformPoint"] = &U::transformPoint;
		}
	}

	void register_mat(sol::state& _lua)
	{
		detail::register_mat3<f32>(_lua, "f32mat3");
		detail::register_mat3<f64>(_lua, "f64mat3");

		detail::register_mat4<f32>(_lua, "f32mat4");
		detail::register_mat4<f64>(_lua, "f64mat4");
	}
}
