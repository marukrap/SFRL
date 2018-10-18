namespace rl
{

constexpr int even(int number)
{
	return number / 2 * 2;
}

constexpr int odd(int number)
{
	return number / 2 * 2 + 1;
}

template <typename T>
constexpr int sign(T value)
{
	return (value > static_cast<T>(0)) - (value < static_cast<T>(0));
}

template <typename T>
constexpr T length(const sf::Vector2<T>& vector)
{
	return static_cast<T>(std::sqrt(vector.x * vector.x + vector.y * vector.y));
}

template <typename T>
constexpr T lengthSquared(const sf::Vector2<T>& vector)
{
	return vector.x * vector.x + vector.y * vector.y;
}

template <typename T>
constexpr float toRadian(const sf::Vector2<T>& vector)
{
	return std::atan2(static_cast<float>(vector.y), static_cast<float>(vector.x));
}

template <typename T>
constexpr float toDegree(const sf::Vector2<T>& vector)
{
	return toRadian(vector) * 180.f / Pi;
}

constexpr float toRadian(float degree)
{
	return Pi / 180.f * degree;
}

constexpr float toDegree(float radian)
{
	return 180.f / Pi * radian;
}

}
