#pragma once

namespace rl
{

// linear interpolation between two values
template <typename T>
constexpr T lerp(T v0, T v1, float t);

// interpolate smoothly between two values
template <typename T>
constexpr T smoothstep(T v0, T v1, float t);

// an improved version of the smoothstep function
template <typename T>
constexpr T smootherstep(T p0, T p1, float t);

// a quadratic bezier curve
template <typename T>
constexpr T bezierCurve(T p0, T p1, T p2, float t);

// a cubic bezier curve
template <typename T>
constexpr T bezierCurve(T p0, T p1, T p2, T p3, float t);

}

#include "Interpolation.inl"
