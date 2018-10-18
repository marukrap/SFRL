#include "GUI/Window.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace rl
{

Window::Window(const sf::Vector2i& size)
	: m_size(size)
	, m_tileSize(TileSize)
{
}

const sf::Vector2i& Window::getSize() const
{
	return m_size;
}

void Window::setSize(const sf::Vector2i& size)
{
	m_size = size;
	m_verticesNeedUpdate = true;
}

void Window::setFrameColor(const sf::Color& color)
{
	m_frameColor = color;
	m_verticesNeedUpdate = true;
}

void Window::setTexture(const sf::Texture& texture, const sf::Vector2i& tileSize, int tileBegin)
{
	m_texture = &texture;
	m_tileSize = tileSize;
	m_tileBegin = tileBegin;
	m_verticesNeedUpdate = true;
}

sf::Text& Window::setTitle(const sf::String& string, const sf::Font& font, int fontSize)
{
	m_title = std::make_unique<sf::Text>(string, font, fontSize);
	setOrigin(0.f, static_cast<float>(m_tileSize.y));

	const sf::FloatRect bounds = m_title->getLocalBounds();
	m_titleLength = static_cast<int>(std::ceil((bounds.left + bounds.width) / m_tileSize.x + 0.5f));

	m_verticesNeedUpdate = true;

	return *m_title;
}

sf::Text& Window::setTitle(const sf::String& string)
{
	return setTitle(string, *getFont(), getFontSize());
}

int Window::getTitleLength() const
{
	return m_titleLength;
}

void Window::updateVertices() const
{
	// UNDONE: auto resize the window
	// const int width = std::max(m_size.x, m_titleLength + 3);

	const int width = m_size.x;
	const int height = m_size.y + (m_title ? 1 : 0);

	m_vertices.clear();

	if (!m_texture)
	{
		const float left   = std::floor(m_tileSize.x / 2.f);
		const float top    = std::floor((m_title ? 1.5f : 0.5f) * m_tileSize.y);
		const float right  = std::floor((width - 0.5f) * m_tileSize.x);
		const float bottom = std::floor((height - 0.5f) * m_tileSize.y);

		const auto addQuad = [this] (float x1, float y1, float x2, float y2)
		{
			m_vertices.emplace_back(sf::Vector2f(x1, y1), m_frameColor);
			m_vertices.emplace_back(sf::Vector2f(x2, y1), m_frameColor);
			m_vertices.emplace_back(sf::Vector2f(x2, y2), m_frameColor);
			m_vertices.emplace_back(sf::Vector2f(x1, y2), m_frameColor);
		};

		constexpr float thickness = 2.f;

		addQuad(left, top, left + thickness, bottom);     // left
		addQuad(right - thickness, top, right, bottom);   // right
		// addQuad(left, top, right, top + thickness);       // top
		addQuad(left, bottom - thickness, right, bottom); // bottom

		// top
		if (m_title)
		{
			/*
			addQuad(left, top, left + 1.5 * m_tileSize.x, top + thickness);
			addQuad(left + (m_titleLength + 1.5f) * m_tileSize.x, top, right, top + thickness);

			const float x = (m_titleLength + 4) * m_tileSize.x / 2.f;
			const float y = 1.5f * m_tileSize.y;
			m_title->setPosition(std::floor(x), std::floor(y));
			*/

			// HACK: left alignment (to fit the grid)
			addQuad(left, top, left + 1.f * m_tileSize.x, top + thickness);
			addQuad(left + (m_titleLength + 1.f) * m_tileSize.x, top, right, top + thickness);

			const float x = 2.f * m_tileSize.x;
			const float y = 1.5f * m_tileSize.y;
			m_title->setPosition(std::floor(x), std::floor(y));
			setAnchorA(*m_title, { 0.f, 0.5f });
		}

		else
			addQuad(left, top, right, top + thickness);
	}

	else
	{
		m_vertices.resize(width * height * 4);

		for (int y = 0; y < height; ++y)
			for (int x = 0; x < width; ++x)
			{
				int tileOffset = 0;

				if (y == 0 && m_titleLength > 0)
				{
					if (x == 0)
						tileOffset += 3;
					else if (x < m_titleLength)
						tileOffset += 4;
					else if (x == m_titleLength)
						tileOffset += 5;
					else
						tileOffset = -1;
				}

				else if (y == 1 && m_titleLength > 0)
				{
					if (x == 0)
						tileOffset += 0x10 + 3;
					else if (x < m_titleLength)
						tileOffset += 0x10 + 4;
					else if (x == m_titleLength)
						tileOffset += 0x10 + 5;
					else if (x == width - 1)
						tileOffset += 2;
					else
						tileOffset += 1;
				}

				else if (y == 0)
				{
					if (x == width - 1)
						tileOffset += 2;
					else if (x != 0)
						tileOffset += 1;
				}

				else if (x == 0)
				{
					if (y == height - 1)
						tileOffset += 0x20;
					else
						tileOffset += 0x10;
				}

				else if (y == height - 1)
				{
					if (x == width - 1)
						tileOffset += 0x20 + 2;
					else
						tileOffset += 0x20 + 1;
				}

				else if (x == width - 1)
					tileOffset += 0x10 + 2;

				else
					tileOffset += 0x10 + 1;

				if (tileOffset < 0)
					continue;

				const auto [tv, tu] = std::div(m_tileBegin + tileOffset, m_texture->getSize().x / m_tileSize.x);

				sf::Vertex* quad = &m_vertices[(x + y * width) * 4];

				quad[0].position = { (x + 0.f) * m_tileSize.x, (y + 0.f) * m_tileSize.y };
				quad[1].position = { (x + 1.f) * m_tileSize.x, (y + 0.f) * m_tileSize.y };
				quad[2].position = { (x + 1.f) * m_tileSize.x, (y + 1.f) * m_tileSize.y };
				quad[3].position = { (x + 0.f) * m_tileSize.x, (y + 1.f) * m_tileSize.y };

				quad[0].texCoords = { (tu + 0.f) * m_tileSize.x, (tv + 0.f) * m_tileSize.y };
				quad[1].texCoords = { (tu + 1.f) * m_tileSize.x, (tv + 0.f) * m_tileSize.y };
				quad[2].texCoords = { (tu + 1.f) * m_tileSize.x, (tv + 1.f) * m_tileSize.y };
				quad[3].texCoords = { (tu + 0.f) * m_tileSize.x, (tv + 1.f) * m_tileSize.y };
			}

		if (m_title)
		{
			const float x = (m_titleLength + 0.5f) * m_tileSize.x / 2.f + 2;
			const float y = m_tileSize.y / 2.f + 3;
			m_title->setPosition(std::floor(x), std::floor(y));
			centerOriginA(*m_title);
		}
	}

	m_verticesNeedUpdate = false;
}

void Window::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_verticesNeedUpdate)
		updateVertices();

	states.transform *= getTransform();
	states.texture = m_texture;
	target.draw(&m_vertices[0], m_vertices.size(), sf::Quads, states);

	if (m_title)
	{
		states.texture = nullptr;
		target.draw(*m_title, states);
	}
}

}
