#pragma once

#include <SFML/System/Vector2.hpp>

#include <vector>

namespace rl
{

// TODO: remove enum
enum class Tile
{
	Unused,
	Floor,
	Corridor,
	Wall,
	ClosedDoor,
	OpenDoor,
	UpStairs,
	DownStairs,
	Water,
	Bridge,
};

class Map
{
public:
	/*
	enum Flags
	{
		None        = 0,
		Passable    = 1 << 0,
		Transparent = 1 << 1,
		Visible     = 1 << 2,
		Explored    = 1 << 3,
	};
	*/

	struct Flags
	{
		bool passable    = false;
		bool transparent = false;
		bool visible     = false;
		bool explored    = false;
	};

public:
	Map(int width, int height);
	explicit Map(const sf::Vector2i& size = { 0, 0 });

	virtual ~Map() = default;

	Map(const Map&) = delete;
	Map& operator=(const Map&) = delete;

	const sf::Vector2i& getSize() const;

	virtual void resize(int width, int height);
	void resize(const sf::Vector2i& size);

	constexpr bool isInBounds(int x, int y) const;
	constexpr bool isInBounds(const sf::Vector2i& position) const;

	Tile getTile(int x, int y) const;
	Tile getTile(const sf::Vector2i& position) const;

	void setTile(int x, int y, Tile tile);
	void setTile(const sf::Vector2i& position, Tile tile);

	Flags& at(int x, int y);
	Flags& at(const sf::Vector2i& position);

	const Flags& at(int x, int y) const;
	const Flags& at(const sf::Vector2i& position) const;

private:
	friend class MapGenerator;

	sf::Vector2i m_size;
	std::vector<Tile> m_tiles;
	std::vector<Flags> m_flags;

public:
	// read-only
	const int& width;
	const int& height;
};

}

#include "Map.inl"
