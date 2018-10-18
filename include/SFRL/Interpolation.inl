namespace rl
{

template <typename T>
constexpr T lerp(T v0, T v1, float t)
{
	return static_cast<T>((1 - t) * v0 + t * v1);
}

template <typename T>
constexpr T smoothstep(T v0, T v1, float t)
{
	return lerp(v0, v1, t * t * (3 - 2 * t));
}

template <typename T>
constexpr T smootherstep(T v0, T v1, float t)
{
	return lerp(v0, v1, t * t * t * (t * (t * 6 - 15) + 10));
}

template <typename T>
constexpr T bezierCurve(T p0, T p1, T p2, float t)
{
	const float u = 1 - t;
	const float uu = u * u;
	const float tt = t * t;

	T p = uu * p0;       // first term
	p += 2 * u * t * p1; // second term
	p += tt * p2;        // third term

	return p;
}

template <typename T>
constexpr T bezierCurve(T p0, T p1, T p2, T p3, float t)
{
	const float u = 1 - t;
	const float uu = u * u;
	const float uuu = uu * u;
	const float tt = t * t;
	const float ttt = tt * t;

	T p = uuu * p0;       // first term
	p += 3 * uu * t * p1; // second term
	p += 3 * u * tt * p2; // third term
	p += ttt * p3;        // fourth term

	return p;
}

}
