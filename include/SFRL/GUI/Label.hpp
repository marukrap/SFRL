#pragma once

#include "Component.hpp"

#include <SFML/Graphics/Text.hpp>

namespace rl
{

// class Console;

class Label : public Component
{
public:
	using Ptr = std::unique_ptr<Label>;

public:
	// explicit Label(const sf::String& string);
	Label(const sf::Font& font, int fontSize, const sf::Vector2i& size = { 0, 0 });
	Label();

	const sf::Vector2i& getSize() const;
	void setSize(const sf::Vector2i& size);

	std::size_t getNumLines() const;

	void setText(const sf::String& string);
	void setTextColor(const sf::Color& color);
	void setTextWrap(bool flag);
	void setTextAlignment(Alignment align);

private:
	void updateTexts() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	// void draw(Console& console);

private:
	const sf::Font* m_font = nullptr;
	const int m_fontSize = 0;
	sf::Vector2i m_size;
	sf::String m_string;
	sf::Color m_color = sf::Color::White;
	bool m_wrap = false;
	mutable std::vector<sf::Text> m_texts;
	mutable bool m_textsNeedUpdate = false;
	Alignment m_alignment = Alignment::Centered;
};

}
