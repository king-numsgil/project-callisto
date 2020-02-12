#pragma once

#include <string_view>
#include <functional>
#include <stdexcept>
#include <string>
#include <array>

using std::function;
using std::string;
using std::literals::operator ""s;
using std::string_view;
using std::literals::operator ""sv;
using std::array;

template<class... Ts> struct overload : Ts ... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

#include <Magnum/Math/Math.h>

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

#include <Magnum/Math/Vector.h>

template<class T> using vec2 = Magnum::Math::Vector2<T>;
template<class T> using vec3 = Magnum::Math::Vector3<T>;
template<class T> using vec4 = Magnum::Math::Vector4<T>;

using i8vec2 = vec2<i8>;
using i8vec3 = vec3<i8>;
using i8vec4 = vec4<i8>;
using u8vec2 = vec2<u8>;
using u8vec3 = vec3<u8>;
using u8vec4 = vec4<u8>;

using i16vec2 = vec2<i16>;
using i16vec3 = vec3<i16>;
using i16vec4 = vec4<i16>;
using u16vec2 = vec2<u16>;
using u16vec3 = vec3<u16>;
using u16vec4 = vec4<u16>;

using i32vec2 = vec2<i32>;
using i32vec3 = vec3<i32>;
using i32vec4 = vec4<i32>;
using u32vec2 = vec2<u32>;
using u32vec3 = vec3<u32>;
using u32vec4 = vec4<u32>;

using i64vec2 = vec2<i64>;
using i64vec3 = vec3<i64>;
using i64vec4 = vec4<i64>;
using u64vec2 = vec2<u64>;
using u64vec3 = vec3<u64>;
using u64vec4 = vec4<u64>;

using f32vec2 = vec2<f32>;
using f32vec3 = vec3<f32>;
using f32vec4 = vec4<f32>;
using f64vec2 = vec2<f64>;
using f64vec3 = vec3<f64>;
using f64vec4 = vec4<f64>;

#include <Magnum/Math/Matrix.h>

template<class T> using mat3 = Magnum::Math::Matrix3<T>;
template<class T> using mat4 = Magnum::Math::Matrix4<T>;

using f32mat3 = mat3<f32>;
using f32mat4 = mat4<f32>;
using f64mat3 = mat3<f64>;
using f64mat4 = mat4<f64>;

template<class T> using mat2x3 = Magnum::Math::Matrix2x3<T>;
template<class T> using mat3x2 = Magnum::Math::Matrix3x2<T>;
template<class T> using mat2x4 = Magnum::Math::Matrix2x3<T>;
template<class T> using mat4x2 = Magnum::Math::Matrix2x3<T>;
template<class T> using mat3x4 = Magnum::Math::Matrix2x3<T>;
template<class T> using mat4x3 = Magnum::Math::Matrix2x3<T>;

using f32mat2x3 = mat2x3<f32>;
using f32mat3x2 = mat3x2<f32>;
using f32mat2x4 = mat2x4<f32>;
using f32mat4x2 = mat4x2<f32>;
using f32mat3x4 = mat3x4<f32>;
using f32mat4x3 = mat4x3<f32>;
using f64mat2x3 = mat2x3<f64>;
using f64mat3x2 = mat3x2<f64>;
using f64mat2x4 = mat2x4<f64>;
using f64mat4x2 = mat4x2<f64>;
using f64mat3x4 = mat3x4<f64>;
using f64mat4x3 = mat4x3<f64>;

#include <Magnum/Math/Quaternion.h>

template<class T> using quat = Magnum::Math::Quaternion<T>;

using f32quat = quat<f32>;
using f64quat = quat<f64>;

#include <Magnum/Math/DualQuaternion.h>

template<class T> using dquat = Magnum::Math::DualQuaternion<T>;

using f32dquat = dquat<f32>;
using f64dquat = dquat<f64>;

#include <Magnum/Math/Complex.h>

template<class T> using comp = Magnum::Math::Complex<T>;

using f32comp = comp<f32>;
using f64comp = comp<f64>;

#include <Magnum/Math/DualComplex.h>

template<class T> using dcomp = Magnum::Math::DualComplex<T>;

using f32dcomp = dcomp<f32>;
using f64dcomp = dcomp<f64>;

#include <Magnum/Math/Color.h>

using u8col3 = Magnum::Math::Color3<u8>;
using u8col4 = Magnum::Math::Color4<u8>;
using f32col3 = Magnum::Math::Color3<f32>;
using f32col4 = Magnum::Math::Color4<f32>;

using Magnum::Math::Literals::operator ""_rgb;
using Magnum::Math::Literals::operator ""_rgbf;
using Magnum::Math::Literals::operator ""_rgba;
using Magnum::Math::Literals::operator ""_rgbaf;
using Magnum::Math::Literals::operator ""_srgb;
using Magnum::Math::Literals::operator ""_srgbf;
using Magnum::Math::Literals::operator ""_srgba;
using Magnum::Math::Literals::operator ""_srgbaf;

#include <Magnum/Math/Constants.h>

using f32const = Magnum::Math::Constants<f32>;
using f64const = Magnum::Math::Constants<f64>;

#include <Magnum/Math/Angle.h>

using f32deg = Magnum::Math::Deg<f32>;
using Magnum::Math::Literals::operator ""_degf;
using f32rad = Magnum::Math::Rad<f32>;
using Magnum::Math::Literals::operator ""_radf;
using f64deg = Magnum::Math::Deg<f64>;
using Magnum::Math::Literals::operator ""_deg;
using f64rad = Magnum::Math::Rad<f64>;
using Magnum::Math::Literals::operator ""_rad;
