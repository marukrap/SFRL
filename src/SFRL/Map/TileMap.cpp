#include "Map/TileMap.hpp"
#include "Map/Map.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>

namespace rl
{

TileMap::TileMap(const sf::Texture& texture, const sf::Vector2i& tileSize)
	: m_texture(&texture)
	, m_tileSize(tileSize)
{
}

void TileMap::setTexture(const sf::Texture& texture, const sf::Vector2i& tileSize)
{
	m_texture = &texture;
	m_tileSize = tileSize;
}

void TileMap::setMap(Map& map)
{
	m_map = &map;
	m_viewRect = { 0, 0, map.width, map.height };
}

void TileMap::setViewRect(const sf::IntRect& rect)
{
	m_viewRect = rect;
	m_verticesNeedUpdate = true;
}

void TileMap::setTiles(const std::vector<int>& tiles)
{
	m_tiles = &tiles;
	m_verticesNeedUpdate = true;
}

void TileMap::setProps(const std::vector<Prop>& props)
{
	m_props = &props;
	m_verticesNeedUpdate = true;
}

void TileMap::setFovHack(bool flag)
{
	m_fovHack = flag;
	m_verticesNeedUpdate = true;
}

void TileMap::updateTileMap()
{
	m_verticesNeedUpdate = true;
}

void TileMap::appendQuad(const sf::Vector2i& position, int tileNumber, const sf::Vector2f& offset) const
{
	const auto [tv, tu] = std::div(tileNumber, m_texture->getSize().x / m_tileSize.x);

	const float x1 = (position.x + 0.f + offset.x) * m_tileSize.x;
	const float y1 = (position.y + 0.f + offset.y) * m_tileSize.y;
	const float x2 = (position.x + 1.f + offset.x) * m_tileSize.x;
	const float y2 = (position.y + 1.f + offset.y) * m_tileSize.y;

	const float u1 = (tu + 0.f) * m_tileSize.x + 0.0625f;
	const float v1 = (tv + 0.f) * m_tileSize.y + 0.0625f;
	const float u2 = (tu + 1.f) * m_tileSize.x - 0.0625f;
	const float v2 = (tv + 1.f) * m_tileSize.y - 0.0625f;

	m_vertices.emplace_back(sf::Vector2f(x1, y1), sf::Vector2f(u1, v1));
	m_vertices.emplace_back(sf::Vector2f(x2, y1), sf::Vector2f(u2, v1));
	m_vertices.emplace_back(sf::Vector2f(x2, y2), sf::Vector2f(u2, v2));
	m_vertices.emplace_back(sf::Vector2f(x1, y2), sf::Vector2f(u1, v2));
}

void TileMap::updateVertices() const
{
	// TODO: improve performance

	assert(m_tiles && m_props);

	m_vertices.clear();
	m_vertices.resize(m_viewRect.width * m_viewRect.height * 4);

	for (int j = 0; j < m_viewRect.height; ++j)
		for (int i = 0; i < m_viewRect.width; ++i)
		{
			const int x = m_viewRect.left + i;
			const int y = m_viewRect.top + j;

			if (!m_map->at(x, y).explored && !m_fovHack)
				continue;

			const auto [tv, tu] = std::div((*m_tiles)[x + y * m_map->width], m_texture->getSize().x / m_tileSize.x);

			sf::Vertex* quad = &m_vertices[(i + j * m_viewRect.width) * 4];

			quad[0].position = { (x + 0.f) * m_tileSize.x, (y + 0.f) * m_tileSize.y };
			quad[1].position = { (x + 1.f) * m_tileSize.x, (y + 0.f) * m_tileSize.y };
			quad[2].position = { (x + 1.f) * m_tileSize.x, (y + 1.f) * m_tileSize.y };
			quad[3].position = { (x + 0.f) * m_tileSize.x, (y + 1.f) * m_tileSize.y };

			// NOTE: half pixel trick to avoid artifacts when scrolling or zooming (0.0625f)
			quad[0].texCoords = { (tu + 0.f) * m_tileSize.x + 0.0625f, (tv + 0.f) * m_tileSize.y + 0.0625f };
			quad[1].texCoords = { (tu + 1.f) * m_tileSize.x - 0.0625f, (tv + 0.f) * m_tileSize.y + 0.0625f };
			quad[2].texCoords = { (tu + 1.f) * m_tileSize.x - 0.0625f, (tv + 1.f) * m_tileSize.y - 0.0625f };
			quad[3].texCoords = { (tu + 0.f) * m_tileSize.x + 0.0625f, (tv + 1.f) * m_tileSize.y - 0.0625f };
		}

	for (const Prop& prop : *m_props)
	{
		if (m_viewRect.contains(prop.position) && (m_map->at(prop.position).explored || m_fovHack))
			appendQuad(prop.position, prop.tileNumber, prop.offset);
	}

	m_verticesNeedUpdate = false;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_verticesNeedUpdate)
		updateVertices();

	states.transform *= getTransform();
	states.texture = m_texture;
	target.draw(&m_vertices[0], m_vertices.size(), sf::Quads, states);
}

}
