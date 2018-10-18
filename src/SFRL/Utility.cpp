#include "Utility.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

// TODO: cross-platform
#include <Windows.h>

#include <random>
#include <cassert>

namespace
{
	std::random_device rd;
	std::mt19937 mt(rd());
}

namespace rl
{

int randomInt(int exclusiveMax)
{
	assert(exclusiveMax > 0);

	return std::uniform_int_distribution<>(0, exclusiveMax - 1)(mt);
}

int randomInt(int min, int inclusiveMax)
{
	assert(min <= inclusiveMax);

	return min + std::uniform_int_distribution<>(0, inclusiveMax - min)(mt);
}

int rollDice(int number, int sides)
{
	assert(number > 0 && sides > 0);

	int result = 0;

	for (int i = 0; i < number; ++i)
		result += randomInt(1, sides);

	return result;
}

int rollDiceHighest(int number, int sides, int highest)
{
	assert(number > 0 && sides > 0 && highest <= number);

	std::vector<int> vector;

	for (int i = 0; i < number; ++i)
		vector.emplace_back(randomInt(1, sides));

	std::sort(vector.begin(), vector.end(), std::greater<int>());

	int result = 0;

	for (int i = 0; i < highest; ++i)
		result += vector[i];

	return result;
}

int rollDiceLowest(int number, int sides, int lowest)
{
	assert(number > 0 && sides > 0 && lowest <= number);

	std::vector<int> vector;

	for (int i = 0; i < number; ++i)
		vector.emplace_back(randomInt(1, sides));

	std::sort(vector.begin(), vector.end());

	int result = 0;

	for (int i = 0; i < lowest; ++i)
		result += vector[i];

	return result;
}

bool startsWith(const std::string& string, const std::string& prefix)
{
	if (prefix.size() > string.size())
		return false;

	return string.compare(0, prefix.size(), prefix) == 0;
}

bool startsWith(const std::wstring& string, const std::wstring& prefix)
{
	if (prefix.size() > string.size())
		return false;

	return string.compare(0, prefix.size(), prefix) == 0;
}

bool endsWith(const std::string& string, const std::string& suffix)
{
	if (suffix.size() > string.size())
		return false;

	return string.compare(string.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool endsWith(const std::wstring& string, const std::wstring& suffix)
{
	if (suffix.size() > string.size())
		return false;

	return string.compare(string.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool contains(const std::string& string, char ch)
{
	return string.find(ch) != std::string::npos;
}

bool contains(const std::wstring& string, wchar_t ch)
{
	return string.find(ch) != std::wstring::npos;
}

bool isVowel(char ch)
{
	return contains("aeiou", ch);
}

bool isVowel(wchar_t ch)
{
	return contains(L"aeiou", ch);
}

bool isVowel(const std::string& string)
{
	return contains("aeiou", string[0]) && !startsWith(string, "uni") && !startsWith(string, "eu");
}

bool isVowel(const std::wstring& string)
{
	return contains(L"aeiou", string[0]) && !startsWith(string, L"uni") && !startsWith(string, L"eu");
}

std::string pluralize(const std::string& string)
{
	std::string result = string;

	switch (string.back())
	{
	case 'e':
		if (endsWith(string, "fe"))
		{
			result.pop_back();
			result.pop_back();
			result += "ve";
		}
		break;

	case 'f':
		if (!endsWith(string, "ff"))
		{
			result.pop_back();
			result += "ve";
		}
		break;

	case 'o':
		if (!isVowel(string[string.back() - 2])) // isConsonant
			result += "e";
		break;

	case 's':
	case 'x':
	case 'z':
		result += "e";
		break;

	case 'h':
		if (endsWith(string, "ch") || endsWith(string, "sh"))
			result += "e";
		break;

	case 'y':
		if (!isVowel(string[string.size() - 2])) // isConsonant
		{
			result.pop_back();
			result += "ie";
		}
		break;
	}

	result += "s";

	return result;
}

std::wstring pluralize(const std::wstring& string)
{
	std::wstring result = string;

	switch (string.back())
	{
	case L'e':
		if (endsWith(string, L"fe"))
		{
			result.pop_back();
			result.pop_back();
			result += L"ve";
		}
		break;

	case L'f':
		if (!endsWith(string, L"ff"))
		{
			result.pop_back();
			result += L"ve";
		}
		break;

	case L'o':
		if (!isVowel(string[string.back() - 2])) // isConsonant
			result += L"e";
		break;

	case L's':
	case L'x':
	case L'z':
		result += L"e";
		break;

	case L'h':
		if (endsWith(string, L"ch") || endsWith(string, L"sh"))
			result += L"e";
		break;

	case L'y':
		if (!isVowel(string[string.size() - 2])) // isConsonant
		{
			result.pop_back();
			result += L"ie";
		}
		break;
	}

	result += L"s";

	return result;
}

std::string trim(const std::string& string)
{
	const std::size_t first = string.find_first_not_of(" \t\n");
	const std::size_t last = string.find_last_not_of(" \t\n");

	if (first == std::string::npos)
		return "";

	return string.substr(first, last - first + 1);
}

std::wstring trim(const std::wstring& string)
{
	const std::size_t first = string.find_first_not_of(L" \t\n");
	const std::size_t last = string.find_last_not_of(L" \t\n");

	if (first == std::string::npos)
		return L"";

	return string.substr(first, last - first + 1);
}

std::wstring ansiToUtf16(const std::string& ansi)
{
	const int utf16Length = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, ansi.data(), ansi.length(), nullptr, 0);
	std::wstring utf16(utf16Length, L'\0');
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, ansi.data(), ansi.length(), &utf16[0], utf16Length);

	return utf16;
}

std::wstring utf8ToUtf16(const std::string& utf8)
{
	const int utf16Length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(), utf8.length(), nullptr, 0);
	std::wstring utf16(utf16Length, L'\0');
	MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(), utf8.length(), &utf16[0], utf16Length);

	return utf16;
}

std::string utf16ToAnsi(const std::wstring& utf16)
{
	const int ansiLength = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, utf16.data(), utf16.length(), nullptr, 0, nullptr, nullptr);
	std::string ansi(ansiLength, '\0');
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, utf16.data(), utf16.length(), &ansi[0], ansiLength, nullptr, nullptr);

	return ansi;
}

std::string utf16ToUtf8(const std::wstring& utf16)
{
	const int utf8Length = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, utf16.data(), utf16.length(), nullptr, 0, nullptr, nullptr);
	std::string utf8(utf8Length, '\0');
	WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, utf16.data(), utf16.length(), &utf8[0], utf8Length, nullptr, nullptr);

	return utf8;
}

std::string ansiToUtf8(const std::string& ansi)
{
	return utf16ToUtf8(ansiToUtf16(ansi));
}

std::string utf8ToAnsi(const std::string& utf8)
{
	return utf16ToAnsi(utf8ToUtf16(utf8));
}

sf::Vector2f unitVector(const sf::Vector2f& vector)
{
	assert(vector != sf::Vector2f(0.f, 0.f));

	return vector / length(vector);
}

void centerOrigin(sf::Sprite& sprite)
{
	const sf::FloatRect bounds = sprite.getLocalBounds();

	sprite.setOrigin(std::floor(bounds.left + bounds.width / 2), std::floor(bounds.top + bounds.height / 2));
}

void centerOrigin(sf::Text& text)
{
	const sf::FloatRect bounds = text.getLocalBounds();

	text.setOrigin(std::floor(bounds.left + bounds.width / 2), std::floor(bounds.top + bounds.height / 2));
}

void centerOriginA(sf::Text& text)
{
	// align the text vertically at cap height and baseline (ignores ascenders/descenders)
	const sf::FloatRect boundsX = text.getLocalBounds();
	sf::FloatRect boundsY = sf::Text('A', *text.getFont(), text.getCharacterSize()).getLocalBounds();

	// UNDONE: multiline text
	/*
	if (!text.getString().isEmpty())
	{
		const std::size_t numLines = std::count(text.getString().begin(), text.getString().end() - 1, L'\n'); // exclude the last character
		const float lineSpacing = text.getFont()->getLineSpacing(text.getCharacterSize());
		const float lineSpacingFactor = text.getLineSpacing();

		boundsY.height += numLines * lineSpacing * lineSpacingFactor;
	}
	*/

	text.setOrigin(std::floor(boundsX.left + boundsX.width / 2), std::floor(boundsY.top + boundsY.height / 2));
}

void setAnchor(sf::Sprite& sprite, const sf::Vector2f& anchor)
{
	const sf::FloatRect bounds = sprite.getLocalBounds();

	const float x = std::floor(bounds.left + bounds.width * anchor.x);
	const float y = std::floor(bounds.top + bounds.height * anchor.y);

	sprite.setOrigin(x, y);
}

void setAnchor(sf::Text& text, const sf::Vector2f& anchor)
{
	const sf::FloatRect bounds = text.getLocalBounds();

	const float x = std::floor(bounds.left + bounds.width * anchor.x);
	const float y = std::floor(bounds.top + bounds.height * anchor.y);

	text.setOrigin(x, y);
}

void setAnchorA(sf::Text& text, const sf::Vector2f& anchor)
{
	const sf::FloatRect boundsX = text.getLocalBounds();
	sf::FloatRect boundsY = sf::Text('A', *text.getFont(), text.getCharacterSize()).getLocalBounds();

	/*
	if (!text.getString().isEmpty())
	{
		const std::size_t numLines = std::count(text.getString().begin(), text.getString().end() - 1, L'\n');
		const float lineSpacing = text.getFont()->getLineSpacing(text.getCharacterSize());
		const float lineSpacingFactor = text.getLineSpacing();

		boundsY.height += numLines * lineSpacing * lineSpacingFactor;
	}
	*/

	const float x = std::floor(boundsX.left + boundsX.width * anchor.x);
	const float y = std::floor(boundsY.top + boundsY.height * anchor.y);

	text.setOrigin(x, y);
}

std::vector<sf::Vector2i> plotLine(const sf::Vector2i& from, const sf::Vector2i& to, bool orthogonalSteps)
{
	const sf::Vector2i delta = to - from;
	sf::Vector2i primaryIncrement(sign(delta.x), 0);
	sf::Vector2i secondaryIncrement(0, sign(delta.y));
	int primary = std::abs(delta.x);
	int secondary = std::abs(delta.y);

	if (secondary > primary)
	{
		std::swap(primary, secondary);
		std::swap(primaryIncrement, secondaryIncrement);
	}

	std::vector<sf::Vector2i> line;
	sf::Vector2i current = from;
	int error = 0;

	while (true)
	{
		line.emplace_back(current);

		if (current == to)
			break;

		current += primaryIncrement;
		error += secondary;

		if (error * 2 >= primary)
		{
			if (orthogonalSteps)
				line.emplace_back(current);

			current += secondaryIncrement;
			error -= primary;
		}
	}

	return line;
}

}
