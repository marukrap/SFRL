#pragma once

#include <SFML/Graphics/Color.hpp>

#include <tuple>
#include <array>

// HSL/HSV
// H Hue component,        range: [0.f, 360.f)
// S Saturation component, range: [0.f, 1.f)
// L Lightness component,  range: [0.f, 1.f)
// V Value component,      range: [0.f, 1.f)

// sf::Color
// r Red component,   range: [0, 255]
// g Green component, range: [0, 255]
// b Blue component,  range: [0, 255]

namespace rl
{

std::tuple<float, float, float> RGBtoHSL(const sf::Color& color);
std::tuple<float, float, float> RGBtoHSV(const sf::Color& color);
sf::Color HSLtoRGB(float H, float S, float L);
sf::Color HSVtoRGB(float H, float S, float V);

// color palettes

const std::array<sf::Color, 8> DB8 =
{
	sf::Color(   0,   0,   0 ), // 0
	sf::Color(  85,  65,  95 ), // 1
	sf::Color( 100, 105, 100 ), // 2
	sf::Color( 215, 115,  85 ), // 3
	sf::Color(  80, 140, 215 ), // 4
	sf::Color( 100, 185, 100 ), // 5
	sf::Color( 230, 200, 110 ), // 6
	sf::Color( 220, 245, 255 ), // 7
};

const std::array<sf::Color, 16> DB16 =
{
	sf::Color(  20,  12,  28 ), //  0
	sf::Color(  68,  36,  52 ), //  1
	sf::Color(  48,  52, 109 ), //  2
	sf::Color(  78,  74,  78 ), //  3
	sf::Color( 133,  76,  48 ), //  4
	sf::Color(  52, 101,  36 ), //  5
	sf::Color( 208,  70,  72 ), //  6
	sf::Color( 117, 113,  97 ), //  7
	sf::Color(  89, 125, 206 ), //  8
	sf::Color( 210, 125,  44 ), //  9
	sf::Color( 133, 149, 161 ), // 10
	sf::Color( 109, 170,  44 ), // 11
	sf::Color( 210, 170, 153 ), // 12
	sf::Color( 109, 194, 202 ), // 13
	sf::Color( 218, 212,  94 ), // 14
	sf::Color( 222, 238, 214 ), // 15
};

const std::array<sf::Color, 32> DB32 =
{
	sf::Color(   0,   0,   0 ), //  0
	sf::Color(  34,  32,  52 ), //  1
	sf::Color(  69,  40,  60 ), //  2
	sf::Color( 102,  57,  49 ), //  3
	sf::Color( 143,  86,  59 ), //  4
	sf::Color( 223, 113,  38 ), //  5
	sf::Color( 217, 160, 102 ), //  6
	sf::Color( 238, 195, 154 ), //  7
	sf::Color( 251, 242,  54 ), //  8
	sf::Color( 153, 229,  80 ), //  9
	sf::Color( 106, 190,  48 ), // 10
	sf::Color(  55, 148, 110 ), // 11
	sf::Color(  75, 105,  47 ), // 12
	sf::Color(  82,  75,  36 ), // 13
	sf::Color(  50,  60,  57 ), // 14
	sf::Color(  63,  63, 116 ), // 15
	sf::Color(  48,  96, 130 ), // 16
	sf::Color(  91, 110, 225 ), // 17
	sf::Color(  99, 155, 255 ), // 18
	sf::Color(  95, 205, 228 ), // 19
	sf::Color( 203, 219, 252 ), // 20
	sf::Color( 255, 255, 255 ), // 21
	sf::Color( 155, 173, 183 ), // 22
	sf::Color( 132, 126, 135 ), // 23
	sf::Color( 105, 106, 106 ), // 24
	sf::Color(  89,  86,  82 ), // 25
	sf::Color( 118,  66, 138 ), // 26
	sf::Color( 172,  50,  50 ), // 27
	sf::Color( 217,  87,  99 ), // 28
	sf::Color( 215, 123, 186 ), // 29
	sf::Color( 143, 151,  74 ), // 30
	sf::Color( 138, 111,  48 ), // 31
};

const std::array<sf::Color, 16> AAP16 =
{
	sf::Color(   7,   7,   8 ), //  0
	sf::Color( 153,  51,  17 ), //  1
	sf::Color( 221, 119,  17 ), //  2
	sf::Color( 255, 255,  51 ), //  3
	sf::Color(  85, 170,  68 ), //  4
	sf::Color(  17,  85,  34 ), //  5
	sf::Color(  68, 238, 187 ), //  6
	sf::Color(  51, 136, 221 ), //  7
	sf::Color(  85,  68, 170 ), //  8
	sf::Color(  51,  34,  34 ), //  9
	sf::Color( 119,  68,  51 ), // 10
	sf::Color( 204, 136,  85 ), // 11
	sf::Color( 255, 221,  85 ), // 12
	sf::Color( 255, 255, 255 ), // 13
	sf::Color( 170, 187, 187 ), // 14
	sf::Color(  85,  85, 119 ), // 15
};

const std::array<sf::Color, 64> AAP64 =
{
	sf::Color(   6,   6,   8 ), //  0
	sf::Color(  20,  16,  19 ), //  1
	sf::Color(  59,  23,  37 ), //  2
	sf::Color( 115,  23,  45 ), //  3
	sf::Color( 180,  32,  42 ), //  4
	sf::Color( 223,  62,  35 ), //  5
	sf::Color( 250, 106,  10 ), //  6
	sf::Color( 249, 163,  27 ), //  7
	sf::Color( 255, 213,  65 ), //  8
	sf::Color( 255, 252,  64 ), //  9
	sf::Color( 214, 242, 100 ), // 10
	sf::Color( 156, 219,  67 ), // 11
	sf::Color(  89, 193,  53 ), // 12
	sf::Color(  20, 160,  46 ), // 13
	sf::Color(  26, 122,  62 ), // 14
	sf::Color(  36,  82,  59 ), // 15
	sf::Color(  18,  32,  32 ), // 16
	sf::Color(  20,  52, 100 ), // 17
	sf::Color(  40,  92, 196 ), // 18
	sf::Color(  36, 159, 222 ), // 19
	sf::Color(  32, 214, 199 ), // 20
	sf::Color( 166, 252, 219 ), // 21
	sf::Color( 255, 255, 255 ), // 22
	sf::Color( 254, 243, 192 ), // 23
	sf::Color( 250, 214, 184 ), // 24
	sf::Color( 245, 160, 151 ), // 25
	sf::Color( 232, 106, 115 ), // 26
	sf::Color( 188,  74, 155 ), // 27
	sf::Color( 121,  58, 128 ), // 28
	sf::Color(  64,  51,  83 ), // 29
	sf::Color(  36,  34,  52 ), // 30
	sf::Color(  34,  28,  26 ), // 31
	sf::Color(  50,  43,  40 ), // 32
	sf::Color( 113,  65,  59 ), // 33
	sf::Color( 187, 117,  71 ), // 34
	sf::Color( 219, 164,  99 ), // 35
	sf::Color( 244, 210, 156 ), // 36
	sf::Color( 218, 224, 234 ), // 37
	sf::Color( 179, 185, 209 ), // 38
	sf::Color( 139, 147, 175 ), // 39
	sf::Color( 109, 117, 141 ), // 40
	sf::Color(  74,  84,  98 ), // 41
	sf::Color(  51,  57,  65 ), // 42
	sf::Color(  66,  36,  51 ), // 43
	sf::Color(  91,  49,  56 ), // 44
	sf::Color( 142,  82,  82 ), // 45
	sf::Color( 186, 117, 106 ), // 46
	sf::Color( 233, 181, 163 ), // 47
	sf::Color( 227, 230, 255 ), // 48
	sf::Color( 185, 191, 251 ), // 49
	sf::Color( 132, 155, 228 ), // 50
	sf::Color(  88, 141, 190 ), // 51
	sf::Color(  71, 125, 133 ), // 52
	sf::Color(  35, 103,  78 ), // 53
	sf::Color(  50, 132, 100 ), // 54
	sf::Color(  93, 175, 141 ), // 55
	sf::Color( 146, 220, 186 ), // 56
	sf::Color( 205, 247, 226 ), // 57
	sf::Color( 228, 210, 170 ), // 58
	sf::Color( 199, 176, 139 ), // 59
	sf::Color( 160, 134,  98 ), // 60
	sf::Color( 121, 103,  85 ), // 61
	sf::Color(  90,  78,  68 ), // 62
	sf::Color(  66,  57,  52 ), // 63
};

}
