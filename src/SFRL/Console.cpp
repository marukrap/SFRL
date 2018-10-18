#include "Console.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

namespace rl
{

Console::Console(const sf::Vector2i& size, const sf::Font& font, int fontSize)
{
	create(size, font, fontSize);
}

void Console::create(const sf::Vector2i& size, const sf::Font& font, int fontSize)
{
	m_size = size;
	m_font = &font;
	m_texture = &font.getTexture(fontSize);
	m_fontSize = fontSize;

	const_cast<sf::Texture*>(m_texture)->setSmooth(false);

	m_tileSize.x = m_font->getGlyph('A', m_fontSize, false).advance;
	m_tileSize.y = m_font->getLineSpacing(m_fontSize);
	std::cout << "Console Tile Size: " << m_tileSize.x << " x " << m_tileSize.y << '\n';

	const sf::FloatRect bounds = sf::Text('A', *m_font, m_fontSize).getLocalBounds();
	const float topSpace = bounds.top;
	const float bottomSpace = m_tileSize.y - (bounds.top + bounds.height);
	m_offset.y = -std::floor((topSpace - bottomSpace) / 2);
	std::cout << "Console Character Offset: " << m_offset.x << ", " << m_offset.y << '\n';

	for (auto& layer : m_layers)
	{
		layer.resize(m_size.x * m_size.y * 4);

		for (int y = 0; y < m_size.y; ++y)
			for (int x = 0; x < m_size.x; ++x)
			{
				sf::Vertex* quad = &layer[(x + y * m_size.x) * 4];

				quad[0] = { { (x + 0.f) * m_tileSize.x, (y + 0.f) * m_tileSize.y }, sf::Color::Transparent };
				quad[1] = { { (x + 1.f) * m_tileSize.x, (y + 0.f) * m_tileSize.y }, sf::Color::Transparent };
				quad[2] = { { (x + 1.f) * m_tileSize.x, (y + 1.f) * m_tileSize.y }, sf::Color::Transparent };
				quad[3] = { { (x + 0.f) * m_tileSize.x, (y + 1.f) * m_tileSize.y }, sf::Color::Transparent };
			}
	}

	// preload ascii glyphs from ' ' to '~'
	for (std::size_t i = 0; i < m_ascii.size(); ++i)
		m_ascii[i] = &m_font->getGlyph(i + 32, m_fontSize, false);

	// TODO: create box-drawing characters here
}

void Console::clear()
{
	for (int i = 0; i < LayerCount; ++i)
		clear(static_cast<Layer>(i));
}

void Console::clear(Layer layer)
{
	if (layer != TextLayer)
		std::for_each(m_layers[layer].begin(), m_layers[layer].end(), [] (auto& v) { v.color = sf::Color::Transparent; });
	else
		std::fill(m_layers[layer].begin(), m_layers[layer].end(), sf::Vertex());
}

void Console::setChar(int x, int y, wchar_t ch, const sf::Color& color)
{
	if (!isInBounds(x, y))
		return;

	const sf::Glyph* glyph = nullptr;

	if (ch >= 32 && ch < 127)
		glyph = m_ascii[ch - 32];
	else
		glyph = &m_font->getGlyph(ch, m_fontSize, false);

	const float x1 = glyph->bounds.left + m_offset.x;
	const float y1 = glyph->bounds.top + m_fontSize + m_offset.y;
	const float x2 = glyph->bounds.left + glyph->bounds.width + m_offset.x;
	const float y2 = glyph->bounds.top + glyph->bounds.height + m_fontSize + m_offset.y;

	const float u1 = static_cast<float>(glyph->textureRect.left);
	const float v1 = static_cast<float>(glyph->textureRect.top);
	const float u2 = static_cast<float>(glyph->textureRect.left + glyph->textureRect.width);
	const float v2 = static_cast<float>(glyph->textureRect.top + glyph->textureRect.height);

	sf::Vertex* quad = &m_layers[TextLayer][(x + y * m_size.x) * 4];

	quad[0] = { { x * m_tileSize.x + x1, y * m_tileSize.y + y1 }, color, { u1, v1 } };
	quad[1] = { { x * m_tileSize.x + x2, y * m_tileSize.y + y1 }, color, { u2, v1 } };
	quad[2] = { { x * m_tileSize.x + x2, y * m_tileSize.y + y2 }, color, { u2, v2 } };
	quad[3] = { { x * m_tileSize.x + x1, y * m_tileSize.y + y2 }, color, { u1, v2 } };
}

void Console::setString(int x, int y, const std::wstring& string, const sf::Color& color)
{
	int dx = 0;
	int dy = 0;

	for (const wchar_t ch : string)
	{
		if (ch == L'\t')
		{
			for (int i = 0; i < 4; ++i)
				setChar(x + dx++, y + dy, L' ', color);
		}

		else if (ch == L'\n')
		{
			dx = 0;
			++dy;
		}

		else
			setChar(x + dx++, y + dy, ch, color);
	}
}

void Console::setColor(int x, int y, const sf::Color& color, Layer layer)
{
	if (!isInBounds(x, y))
		return;

	sf::Vertex* quad = &m_layers[layer][(x + y * m_size.x) * 4];

	quad[0].color = color;
	quad[1].color = color;
	quad[2].color = color;
	quad[3].color = color;
}

void Console::setColor(int left, int top, int width, int height, const sf::Color& color, Layer layer)
{
	for (int y = top; y < top + height; ++y)
		for (int x = left; x < left + width; ++x)
			setColor(x, y, color, layer);
}

void Console::setColorA(int x, int y, const sf::Uint8 alpha, Layer layer)
{
	if (!isInBounds(x, y))
		return;

	sf::Vertex* quad = &m_layers[layer][(x + y * m_size.x) * 4];

	quad[0].color.a = alpha;
	quad[1].color.a = alpha;
	quad[2].color.a = alpha;
	quad[3].color.a = alpha;
}

void Console::setColorA(const sf::IntRect& rect, const sf::Uint8 alpha, Layer layer)
{
	for (int y = rect.top; y < rect.top + rect.height; ++y)
		for (int x = rect.left; x < rect.left + rect.width; ++x)
			setColorA(x, y, alpha, layer);
}

void Console::setOffsetX(float offsetX)
{
	m_offset.x = offsetX;
}

const sf::Vector2i& Console::getSize() const
{
	return m_size;
}

const sf::Vector2f& Console::getTileSize() const
{
	return m_tileSize;
}

void Console::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (int i = 0; i < LayerCount; ++i)
	{
		states.texture = (i == TextLayer ? m_texture : nullptr);
		target.draw(&m_layers[i][0], m_layers[i].size(), sf::Quads, states);
	}
}

}
