#pragma once

// TODO: more easing functions

#include <cmath>

namespace rl
{

template <typename T>
constexpr T easeInExpo(T v0, T v1, float t);

template <typename T>
constexpr T easeOutExpo(T v0, T v1, float t);

template <typename T>
constexpr T easeInOutExpo(T v0, T v1, float t);

}

#include "Easing.inl"
