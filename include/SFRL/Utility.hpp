#pragma once

// TODO: template functions for string operations (std::basic_string)
//       and try to use std::string_view

#include <SFML/System/Vector2.hpp>

#include <string>
#include <vector>

namespace sf
{
	class Sprite;
	class Text;
}

namespace rl
{

// random number generation
int randomInt(int exclusiveMax);          // [0, max)
int randomInt(int min, int inclusiveMax); // [min, max]

int rollDice(int number, int sides);      // roll s sided dice n times
int rollDiceHighest(int number, int sides, int highest);
int rollDiceLowest(int number, int sides, int lowest);

// string operations
bool startsWith(const std::string& string, const std::string& prefix);
bool startsWith(const std::wstring& string, const std::wstring& prefix);

bool endsWith(const std::string& string, const std::string& suffix);
bool endsWith(const std::wstring& string, const std::wstring& suffix);

bool contains(const std::string& string, char ch);
bool contains(const std::wstring& string, wchar_t ch);

bool isVowel(char ch);
bool isVowel(wchar_t ch);
bool isVowel(const std::string& string);
bool isVowel(const std::wstring& string);

std::string pluralize(const std::string& string);
std::wstring pluralize(const std::wstring& string);

std::string trim(const std::string& string);
std::wstring trim(const std::wstring& string);

// unicode encoding conversions
std::wstring ansiToUtf16(const std::string& ansi);
std::wstring utf8ToUtf16(const std::string& utf8);

std::string utf16ToAnsi(const std::wstring& utf16);
std::string utf16ToUtf8(const std::wstring& utf16);

std::string ansiToUtf8(const std::string& ansi);
std::string utf8ToAnsi(const std::string& utf8);

// center objects
void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOriginA(sf::Text& text);

void setAnchor(sf::Sprite& sprite, const sf::Vector2f& anchor);
void setAnchor(sf::Text& text, const sf::Vector2f& anchor);
void setAnchorA(sf::Text& text, const sf::Vector2f& anchor);

//
constexpr int even(int number);
constexpr int odd(int number);

// type-safe sign function
template <typename T>
constexpr int sign(T value);

// vector operations
sf::Vector2f unitVector(const sf::Vector2f& vector);

template <typename T>
constexpr T length(const sf::Vector2<T>& vector);
template <typename T>
constexpr T lengthSquared(const sf::Vector2<T>& vector);

template <typename T>
constexpr float toRadian(const sf::Vector2<T>& vector);
template <typename T>
constexpr float toDegree(const sf::Vector2<T>& vector);

constexpr float toRadian(float degree);
constexpr float toDegree(float radian);

// bresenham's line algorithm
std::vector<sf::Vector2i> plotLine(const sf::Vector2i& from, const sf::Vector2i& to, bool orthogonalSteps = false);

constexpr float Pi = 3.14159265358979323846f;

}

#include "Utility.inl"
