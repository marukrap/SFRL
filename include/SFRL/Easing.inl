namespace rl
{

template <typename T>
constexpr T easeInExpo(T v0, T v1, float t)
{
	return static_cast<T>((v1 - v0) * std::pow(2, 10 * (t - 1)) + v0);
}

template <typename T>
constexpr T easeOutExpo(T v0, T v1, float t)
{
	return static_cast<T>((v1 - v0) * (-std::pow(2, -10 * t) + 1) + v0);
}

template <typename T>
constexpr T easeInOutExpo(T v0, T v1, float t)
{
	if (t < 0.5f)
		return static_cast<T>((v1 - v0) / 2 * std::pow(2, 10 * (t * 2 - 1)) + v0);
	else
		return static_cast<T>((v1 - v0) / 2 * (-std::pow(2, -10 * (t * 2 - 1)) + 2) + v0);
}

}
