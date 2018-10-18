#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <array>
#include <vector>

namespace sf
{
	class Font;
	class Glyph;
}

namespace rl
{

class Console : public sf::Drawable, public sf::Transformable
{
public:
	enum /* class */ Layer
	{
		Background,
		TextLayer,
		Foreground,
		LayerCount,
	};

public:
	Console() = default;
	Console(const sf::Vector2i& size, const sf::Font& font, int fontSize);

	void create(const sf::Vector2i& size, const sf::Font& font, int fontSize);

	void clear();
	void clear(Layer layer);

	void setChar(int x, int y, wchar_t ch, const sf::Color& color = sf::Color::White);
	void setString(int x, int y, const std::wstring& string, const sf::Color& color = sf::Color::White);

	void setColor(int x, int y, const sf::Color& color, Layer layer = Background);
	void setColor(int left, int top, int width, int height, const sf::Color& color, Layer layer = Background);

	void setColorA(int x, int y, const sf::Uint8 alpha, Layer layer = Background);
	void setColorA(const sf::IntRect& rect, const sf::Uint8 alpha, Layer layer = Background);

	void setOffsetX(float offsetX);

	const sf::Vector2i& getSize() const;
	const sf::Vector2f& getTileSize() const;

private:
	constexpr bool isInBounds(int x, int y) const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	const sf::Font* m_font = nullptr;
	const sf::Texture* m_texture = nullptr;
	int m_fontSize = 0;
	sf::Vector2i m_size;
	sf::Vector2f m_tileSize;
	sf::Vector2f m_offset;
	std::array<const sf::Glyph*, 95> m_ascii;
	std::array<std::vector<sf::Vertex>, LayerCount> m_layers;
};

constexpr bool Console::isInBounds(int x, int y) const
{
	return x >= 0 && y >= 0 && x < m_size.x && y < m_size.y;
}

}
