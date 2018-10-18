namespace rl
{

constexpr bool Map::isInBounds(int x, int y) const
{
	return x >= 0 && y >= 0 && x < width && y < height;
}

constexpr bool Map::isInBounds(const sf::Vector2i& position) const
{
	return isInBounds(position.x, position.y);
}

inline Tile Map::getTile(int x, int y) const
{
	return m_tiles[x + y * width];
}

inline Tile Map::getTile(const sf::Vector2i& position) const
{
	return getTile(position.x, position.y);
}

inline void Map::setTile(int x, int y, Tile tile)
{
	m_tiles[x + y * width] = tile;
}

inline void Map::setTile(const sf::Vector2i& position, Tile tile)
{
	setTile(position.x, position.y, tile);
}

inline Map::Flags& Map::at(int x, int y)
{
	return m_flags[x + y * width];
}

inline Map::Flags& Map::at(const sf::Vector2i& position)
{
	return at(position.x, position.y);
}

inline const Map::Flags& Map::at(int x, int y) const
{
	return m_flags[x + y * width];
}

inline const Map::Flags& Map::at(const sf::Vector2i& position) const
{
	return at(position.x, position.y);
}

}
