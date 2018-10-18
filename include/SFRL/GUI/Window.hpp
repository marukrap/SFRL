#pragma once

#include "Component.hpp"

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Text.hpp>

namespace rl
{

class Window : public Component
{
public:
	using Ptr = std::unique_ptr<Window>;

public:
	explicit Window(const sf::Vector2i& size = { 0, 0 });

	const sf::Vector2i& getSize() const;
	void setSize(const sf::Vector2i& size);

	void setFrameColor(const sf::Color& color);
	void setTexture(const sf::Texture& texture, const sf::Vector2i& tileSize, int tileBegin = 0);
	sf::Text& setTitle(const sf::String& string, const sf::Font& font, int fontSize);
	sf::Text& setTitle(const sf::String& string);

	int getTitleLength() const;

private:
	void updateVertices() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	const sf::Texture* m_texture = nullptr;
	sf::Vector2i m_size;
	sf::Vector2i m_tileSize;
	sf::Color m_frameColor = sf::Color::White;
	std::unique_ptr<sf::Text> m_title = nullptr;
	int m_tileBegin = 0;
	int m_titleLength = 0;
	mutable std::vector<sf::Vertex> m_vertices;
	mutable bool m_verticesNeedUpdate = true;
};

}
