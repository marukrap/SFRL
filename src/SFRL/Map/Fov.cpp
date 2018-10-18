#include "Map/Fov.hpp"
#include "Map/Map.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace rl
{

bool Fov::Shadow::contains(const Shadow& projection) const
{
	return start <= projection.start && end >= projection.end;
}

Fov::Fov(const sf::Vector2i& tileSize, const sf::Texture* texture, int tileBegin)
	: m_texture(texture)
	, m_tileSize(tileSize)
	, m_tileBegin(tileBegin)
{
}

void Fov::setTexture(const sf::Vector2i& tileSize, const sf::Texture* texture, int tileBegin)
{
	m_texture = texture;
	m_tileSize = tileSize;
	m_tileBegin = tileBegin;
}

void Fov::setMap(Map& map)
{
	m_map = &map;
	m_viewRect = { 0, 0, map.width, map.height };
}

void Fov::setViewRect(const sf::IntRect& rect)
{
	m_viewRect = rect;
	m_verticesNeedUpdate = true;
}

void Fov::clear()
{
	for (int y = 0; y < m_map->height; ++y)
		for (int x = 0; x < m_map->width; ++x)
			m_map->at(x, y).visible = false;

	m_verticesNeedUpdate = true;
}

void Fov::compute(const sf::Vector2i& position, int range)
{
	// clear();

	if (range >= 0)
	{
		m_map->at(position).visible = true;
		m_map->at(position).explored = true;

		for (int octant = 0; octant < 8; ++octant)
			refreshOctant(octant, position, range + 1);

		m_verticesNeedUpdate = true;
	}
}

void Fov::refreshOctant(int octant, const sf::Vector2i& start, int range)
{
	sf::Vector2i rowInc;
	sf::Vector2i colInc;

	switch (octant)
	{
	case 0: rowInc = {  0, -1 }; colInc = {  1,  0 }; break;
	case 1: rowInc = {  1,  0 }; colInc = {  0, -1 }; break;
	case 2: rowInc = {  1,  0 }; colInc = {  0,  1 }; break;
	case 3: rowInc = {  0,  1 }; colInc = {  1,  0 }; break;
	case 4: rowInc = {  0,  1 }; colInc = { -1,  0 }; break;
	case 5: rowInc = { -1,  0 }; colInc = {  0,  1 }; break;
	case 6: rowInc = { -1,  0 }; colInc = {  0, -1 }; break;
	case 7: rowInc = {  0, -1 }; colInc = { -1,  0 }; break;
	}

	m_shadows.clear();

	bool fullShadow = false;

	for (int row = 1; row < range; ++row)
	{
		sf::Vector2i pos = start + (rowInc * row);

		if (!m_map->isInBounds(pos))
			break;

		for (int col = 0; col <= row; ++col)
		{
			// circular fov
			if (lengthSquared(pos - start) >= range * range)
				break;

			// if (fullShadow)
				// m_map->at(pos).visible = false;

			// else
			// {
				const Shadow projection = getProjection(col, row);

				if (!isInShadow(projection)) // visible
				{
					m_map->at(pos).visible = true;
					m_map->at(pos).explored = true;

					if (!m_map->at(pos).transparent) // wall (blocks light)
						fullShadow = addShadow(projection);
				}

				// else
					// m_map->at(pos).visible = false;
			// }

			pos += colInc;

			if (!m_map->isInBounds(pos))
				break;
		}
	}
}

Fov::Shadow Fov::getProjection(int col, int row)
{
	const float topLeft = static_cast<float>(col) / (row + 2);
	const float bottomRight = static_cast<float>(col + 1) / (row + 1);

	return { topLeft, bottomRight };
}

bool Fov::isInShadow(const Shadow& projection) const
{
	for (const auto& shadow : m_shadows)
	{
		if (shadow.contains(projection))
			return true;
	}

	return false;
}

bool Fov::addShadow(const Shadow& shadow)
{
	std::size_t index = 0;

	for (; index < m_shadows.size(); ++index)
	{
		// NOTE: > or >=
		if (m_shadows[index].start > shadow.start)
			break;
	}

	const bool overlapsPrev = ((index > 0) && (m_shadows[index - 1].end > shadow.start));
	const bool overlapsNext = ((index < m_shadows.size()) && (m_shadows[index].start < shadow.end));

	if (overlapsNext)
	{
		if (overlapsPrev)
		{
			m_shadows[index - 1].end = std::max(m_shadows[index - 1].end, m_shadows[index].end);
			m_shadows.erase(m_shadows.begin() + index);
		}

		else
			m_shadows[index].start = std::min(m_shadows[index].start, shadow.start);
	}

	else
	{
		if (overlapsPrev)
			m_shadows[index - 1].end = std::max(m_shadows[index - 1].end, shadow.end);
		else
			m_shadows.emplace(m_shadows.begin() + index, shadow);
	}

	return (m_shadows.size() == 1) && (m_shadows[0].start == 0.f) && (m_shadows[0].end == 1.f);
}

bool Fov::isVisible(int x, int y) const
{
	// return m_map->isInBounds(x, y) && m_map->at(x, y).visible;
	return !m_map->isInBounds(x, y) || m_map->at(x, y).visible;
}

bool Fov::isExplored(int x, int y) const
{
	// return m_map->isInBounds(x, y) && m_map->at(x, y).explored;
	return !m_map->isInBounds(x, y) || m_map->at(x, y).explored;
}

void Fov::appendQuad(int x, int y, int tileOffset, const sf::Color& color) const
{
	const auto [tv, tu] = std::div(m_tileBegin + tileOffset, m_texture->getSize().x / m_tileSize.x);

	const float x1 = (x + 0.f) * m_tileSize.x;
	const float y1 = (y + 0.f) * m_tileSize.y;
	const float x2 = (x + 1.f) * m_tileSize.x;
	const float y2 = (y + 1.f) * m_tileSize.y;

	const float u1 = (tu + 0.f) * m_tileSize.x + 0.0625f;
	const float v1 = (tv + 0.f) * m_tileSize.y + 0.0625f;
	const float u2 = (tu + 1.f) * m_tileSize.x - 0.0625f;
	const float v2 = (tv + 1.f) * m_tileSize.y - 0.0625f;

	m_vertices.emplace_back(sf::Vector2f(x1, y1), color, sf::Vector2f(u1, v1));
	m_vertices.emplace_back(sf::Vector2f(x2, y1), color, sf::Vector2f(u2, v1));
	m_vertices.emplace_back(sf::Vector2f(x2, y2), color, sf::Vector2f(u2, v2));
	m_vertices.emplace_back(sf::Vector2f(x1, y2), color, sf::Vector2f(u1, v2));
}

void Fov::updateVertices() const
{
	m_vertices.clear();
	m_vertices.resize(m_viewRect.width * m_viewRect.height * 4);

	if (!m_texture)
	{
		for (int j = 0; j < m_viewRect.height; ++j)
			for (int i = 0; i < m_viewRect.width; ++i)
			{
				const int x = m_viewRect.left + i;
				const int y = m_viewRect.top + j;

				sf::Color color(0, 0, 0);

				if (m_map->at(x, y).visible)
					continue; // color.a = 0;
				else if (m_map->at(x, y).explored)
					color.a = 204;

				sf::Vertex* quad = &m_vertices[(i + j * m_viewRect.width) * 4];

				quad[0].position = { (x + 0.f) * m_tileSize.x, (y + 0.f) * m_tileSize.y };
				quad[1].position = { (x + 1.f) * m_tileSize.x, (y + 0.f) * m_tileSize.y };
				quad[2].position = { (x + 1.f) * m_tileSize.x, (y + 1.f) * m_tileSize.y };
				quad[3].position = { (x + 0.f) * m_tileSize.x, (y + 1.f) * m_tileSize.y };

				quad[0].color = color;
				quad[1].color = color;
				quad[2].color = color;
				quad[3].color = color;
			}
	}

	else
	{
		// TODO: improve performance

		for (int j = 0; j < m_viewRect.height; ++j)
			for (int i = 0; i < m_viewRect.width; ++i)
			{
				const int x = m_viewRect.left + i;
				const int y = m_viewRect.top + j;

				int tileOffset = 15; // opaque
				sf::Color color(255, 255, 255);

				if (m_map->at(x, y).visible)
				{
					int visible = 0;

					if (isVisible(x - 1, y))
						visible += 1;
					if (isVisible(x + 1, y))
						visible += 2;
					if (isVisible(x, y - 1))
						visible += 4;
					if (isVisible(x, y + 1))
						visible += 8;

					if (visible != 15)
						appendQuad(x, y, visible, { 255, 255, 255, 204 });

					if (visible & 1)
					{
						if ((visible & 4) && !isVisible(x - 1, y - 1))
							appendQuad(x, y, 16, { 255, 255, 255, 204 });
						if ((visible & 8) && !isVisible(x - 1, y + 1))
							appendQuad(x, y, 16 + 1, { 255, 255, 255, 204 });
					}

					if (visible & 2)
					{
						if ((visible & 4) && !isVisible(x + 1, y - 1))
							appendQuad(x, y, 16 + 2, { 255, 255, 255, 204 });
						if ((visible & 8) && !isVisible(x + 1, y + 1))
							appendQuad(x, y, 16 + 3, { 255, 255, 255, 204 });
					}

					int explored = 0;

					if (isExplored(x - 1, y))
						explored += 1;
					if (isExplored(x + 1, y))
						explored += 2;
					if (isExplored(x, y - 1))
						explored += 4;
					if (isExplored(x, y + 1))
						explored += 8;

					if (explored != 15)
						appendQuad(x, y, explored);

					if (explored & 1)
					{
						if ((explored & 4) && !isExplored(x - 1, y - 1))
							appendQuad(x, y, 16);
						if ((explored & 8) && !isExplored(x - 1, y + 1))
							appendQuad(x, y, 16 + 1);
					}

					if (explored & 2)
					{
						if ((explored & 4) && !isExplored(x + 1, y - 1))
							appendQuad(x, y, 16 + 2);
						if ((explored & 8) && !isExplored(x + 1, y + 1))
							appendQuad(x, y, 16 + 3);
					}

					continue; // color.a = 0;
				}

				else if (m_map->at(x, y).explored)
				{
					color.a = 204;

					int explored = 0;

					if (isExplored(x - 1, y))
						explored += 1;
					if (isExplored(x + 1, y))
						explored += 2;
					if (isExplored(x, y - 1))
						explored += 4;
					if (isExplored(x, y + 1))
						explored += 8;

					if (explored != 15)
						appendQuad(x, y, explored);

					if (explored & 1)
					{
						if ((explored & 4) && !isExplored(x - 1, y - 1))
							appendQuad(x, y, 16);
						if ((explored & 8) && !isExplored(x - 1, y + 1))
							appendQuad(x, y, 16 + 1);
					}

					if (explored & 2)
					{
						if ((explored & 4) && !isExplored(x + 1, y - 1))
							appendQuad(x, y, 16 + 2);
						if ((explored & 8) && !isExplored(x + 1, y + 1))
							appendQuad(x, y, 16 + 3);
					}
				}

				// if (tileOffset < 0)
					// continue;

				const auto [tv, tu] = std::div(m_tileBegin + tileOffset, m_texture->getSize().x / m_tileSize.x);

				sf::Vertex* quad = &m_vertices[(i + j * m_viewRect.width) * 4];

				quad[0].position = { (x + 0.f) * m_tileSize.x, (y + 0.f) * m_tileSize.y };
				quad[1].position = { (x + 1.f) * m_tileSize.x, (y + 0.f) * m_tileSize.y };
				quad[2].position = { (x + 1.f) * m_tileSize.x, (y + 1.f) * m_tileSize.y };
				quad[3].position = { (x + 0.f) * m_tileSize.x, (y + 1.f) * m_tileSize.y };

				quad[0].color = color;
				quad[1].color = color;
				quad[2].color = color;
				quad[3].color = color;

				// NOTE: half pixel trick to avoid artifacts when scrolling or zooming (0.0625f)
				quad[0].texCoords = { (tu + 0.f) * m_tileSize.x + 0.0625f, (tv + 0.f) * m_tileSize.y + 0.0625f };
				quad[1].texCoords = { (tu + 1.f) * m_tileSize.x - 0.0625f, (tv + 0.f) * m_tileSize.y + 0.0625f };
				quad[2].texCoords = { (tu + 1.f) * m_tileSize.x - 0.0625f, (tv + 1.f) * m_tileSize.y - 0.0625f };
				quad[3].texCoords = { (tu + 0.f) * m_tileSize.x + 0.0625f, (tv + 1.f) * m_tileSize.y - 0.0625f };
			}
	}

	m_verticesNeedUpdate = false;
}

void Fov::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_verticesNeedUpdate)
		updateVertices();

	states.transform *= getTransform();
	states.texture = m_texture;
	target.draw(&m_vertices[0], m_vertices.size(), sf::Quads, states);
}

}
