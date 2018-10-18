#include "Map/Map.hpp"

// #include <algorithm>

namespace rl
{

Map::Map(int width, int height)
	: width(m_size.x)
	, height(m_size.y)
{
	resize(width, height);
}

Map::Map(const sf::Vector2i& size)
	: Map(size.x, size.y)
{
}

const sf::Vector2i& Map::getSize() const
{
	return m_size;
}

void Map::resize(int width, int height)
{
	m_size = { width, height };
	m_tiles.resize(width * height);
	m_flags.resize(width * height);
}

void Map::resize(const sf::Vector2i& size)
{
	resize(size.x, size.y);
}

/*
bool Map::hasFlag(int x, int y, Flags flag) const
{
	return (m_flags[x + y * width] & flag) != Flags::None;
}

bool Map::hasFlag(const sf::Vector2i& position, Flags flag) const
{
	return hasFlag(position.x, position.y, flag);
}

void Map::addFlag(int x, int y, Flags flag)
{
	m_flags[x + y * width] |= flag;
}

void Map::addFlag(const sf::Vector2i& position, Flags flag)
{
	addFlag(position.x, position.y, flag);
}

void Map::addFlag(Flags flag)
{
	std::for_each(m_flags.begin(), m_flags.end(), [&] (auto& f) { f |= flag; });
}

void Map::removeFlag(int x, int y, Flags flag)
{
	m_flags[x + y * width] &= ~flag;
}

void Map::removeFlag(const sf::Vector2i& position, Flags flag)
{
	removeFlag(position.x, position.y, flag);
}

void Map::removeFlag(Flags flag)
{
	std::for_each(m_flags.begin(), m_flags.end(), [&] (auto& f) { f &= ~flag; });
}

void Map::resetFlags(int x, int y, Flags flags)
{
	m_flags[x + y * width] = flags;
}

void Map::resetFlags(const sf::Vector2i& position, Flags flags)
{
	resetFlags(position.x, position.y, flags);
}

void Map::resetFlags(Flags flags)
{
	std::fill(m_flags.begin(), m_flags.end(), flags);
}

Map::Flags operator~(Map::Flags flag)
{
	return static_cast<Map::Flags>(~static_cast<int>(flag));
}

Map::Flags operator|(Map::Flags lhs, Map::Flags rhs)
{
	return static_cast<Map::Flags>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

Map::Flags operator&(Map::Flags lhs, Map::Flags rhs)
{
	return static_cast<Map::Flags>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

Map::Flags& operator|=(Map::Flags& lhs, Map::Flags rhs)
{
	return lhs = lhs | rhs;
}

Map::Flags& operator&=(Map::Flags& lhs, Map::Flags rhs)
{
	return lhs = lhs & rhs;
}
*/

}
