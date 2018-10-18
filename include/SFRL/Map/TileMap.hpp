#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <vector>

namespace rl
{

class Map;

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	struct Prop
	{
		int tileNumber;
		sf::Vector2i position;
		sf::Vector2f offset = { 0.f, 0.f };
	};

public:
	TileMap() = default;
	TileMap(const sf::Texture& texture, const sf::Vector2i& tileSize);

	TileMap(const TileMap&) = delete;
	TileMap& operator=(const TileMap&) = delete;

	void setTexture(const sf::Texture& texture, const sf::Vector2i& tileSize);
	void setMap(Map& map);
	void setViewRect(const sf::IntRect& rect);

	void setTiles(const std::vector<int>& tiles);
	void setProps(const std::vector<Prop>& props);
	void setFovHack(bool flag);

	void updateTileMap();

private:
	void appendQuad(const sf::Vector2i& position, int tileNumber, const sf::Vector2f& offset) const;
	void updateVertices() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	const sf::Texture* m_texture = nullptr;
	sf::Vector2i m_tileSize;
	const Map* m_map = nullptr;
	const std::vector<int>* m_tiles = nullptr; // background
	const std::vector<Prop>* m_props = nullptr; // foreground
	sf::IntRect m_viewRect;
	bool m_fovHack = false;
	mutable std::vector<sf::Vertex> m_vertices;
	mutable bool m_verticesNeedUpdate = false;
};

}
