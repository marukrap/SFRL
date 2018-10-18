#include "Color.hpp"

#include <algorithm>

// https://en.wikipedia.org/wiki/HSL_and_HSV

namespace rl
{

std::tuple<float, float, float> RGBtoHSL(const sf::Color& color)
{
	const float R = color.r / 255.f;
	const float G = color.g / 255.f;
	const float B = color.b / 255.f;

	const float M = std::max({ R, G, B });
	const float m = std::min({ R, G, B });
	const float C = M - m; // chroma

	float H = 0.f; // hue
	float S = 0.f; // saturation
	float L = 0.f; // lightness

	if (C != 0.f)
	{
		if (M == R)
			H = std::fmod(((G - B) / C), 6.f);
		else if (M == G)
			H = ((B - R) / C) + 2;
		else if (M == B)
			H = ((R - G) / C) + 4;

		H *= 60;
	}

	if (H < 0.f)
		H += 360;

	L += (M + m) / 2;

	if (L != 1.f && L != 0.f)
		S = C / (1 - std::fabs(2 * L - 1));

	return { H, S, L };
}

std::tuple<float, float, float> RGBtoHSV(const sf::Color& color)
{
	const float R = color.r / 255.f;
	const float G = color.g / 255.f;
	const float B = color.b / 255.f;

	const float M = std::max({ R, G, B });
	const float m = std::min({ R, G, B });
	const float C = M - m; // chroma

	float H = 0.f; // hue
	float S = 0.f; // saturation
	float V = 0.f; // value

	if (C != 0.f)
	{
		if (M == R)
			H = std::fmod(((G - B) / C), 6.f);
		else if (M == G)
			H = ((B - R) / C) + 2;
		else if (M == B)
			H = ((R - G) / C) + 4;

		H *= 60;
	}

	if (H < 0.f)
		H += 360;

	V = M;

	if (V != 0.f)
		S = C / V;

	return { H, S, V };
}

sf::Color HSLtoRGB(float H, float S, float L)
{
	const float C = (1 - std::fabs(2 * L - 1)) * S; // chroma
	const float HPrime = H / 60; // H'
	const float X = C * (1 - std::fabs(std::fmod(HPrime, 2.f) - 1));
	const float M = L - C / 2;

	float R = 0.f;
	float G = 0.f;
	float B = 0.f;

	switch (static_cast<int>(HPrime))
	{
	case 0: R = C; G = X;        break; // [0, 1)
	case 1: R = X; G = C;        break; // [1, 2)
	case 2:        G = C; B = X; break; // [2, 3)
	case 3:        G = X; B = C; break; // [3, 4)
	case 4: R = X;        B = C; break; // [4, 5)
	case 5: R = C;        B = X; break; // [5, 6)
	}

	R += M;
	G += M;
	B += M;

	sf::Color color;
	color.r = static_cast<sf::Uint8>(std::round(R * 255));
	color.g = static_cast<sf::Uint8>(std::round(G * 255));
	color.b = static_cast<sf::Uint8>(std::round(B * 255));

	return color;
}

sf::Color HSVtoRGB(float H, float S, float V)
{
	const float C = S * V; // chroma
	const float HPrime = std::fmod(H / 60, 6.f); // H'
	const float X = C * (1 - std::fabs(std::fmod(HPrime, 2.f) - 1));
	const float M = V - C;

	float R = 0.f;
	float G = 0.f;
	float B = 0.f;

	switch (static_cast<int>(HPrime))
	{
	case 0: R = C; G = X;        break; // [0, 1)
	case 1: R = X; G = C;        break; // [1, 2)
	case 2:        G = C; B = X; break; // [2, 3)
	case 3:        G = X; B = C; break; // [3, 4)
	case 4: R = X;        B = C; break; // [4, 5)
	case 5: R = C;        B = X; break; // [5, 6)
	}

	R += M;
	G += M;
	B += M;

	sf::Color color;
	color.r = static_cast<sf::Uint8>(std::round(R * 255));
	color.g = static_cast<sf::Uint8>(std::round(G * 255));
	color.b = static_cast<sf::Uint8>(std::round(B * 255));

	return color;
}

}
