#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>

#include <memory>

namespace sf
{
	class Event;
	class Font;
}

namespace rl // rl::gui
{

enum class Alignment
{
	Centered,
	Left,
	Right,
};

class Component : public sf::Drawable, public sf::Transformable
{
public:
	using Ptr = std::unique_ptr<Component>;

public:
	Component() = default;
	virtual ~Component() = default;

	Component(const Component&) = delete;
	Component& operator=(const Component&) = delete;

	const sf::Vector2i& getPosition() const;
	void setPosition(int x, int y);
	void setPosition(const sf::Vector2i& position);

	void move(int x, int y);
	void move(const sf::Vector2i& offset);

	virtual bool isSelectable() const;

	bool isSelected() const;
	void select();
	void deselect();

	bool isActive() const;
	void activate();
	void deactivate();

	virtual bool contains(const sf::Vector2f& point) const;
	virtual void handleEvent(const sf::Event& event);
	virtual void update(sf::Time dt);

	static void setTileSize(const sf::Vector2i& tileSize);
	static void setFont(const sf::Font& font);
	static void setFontSize(unsigned int fontSize);

protected:
	const sf::Font* getFont() const;
	unsigned int getFontSize() const;

private:
	virtual void onSelect();
	virtual void onDeselect();

	virtual void onActivate();
	virtual void onDeactivate();

private:
	static sf::Vector2i s_tileSize;
	static const sf::Font* s_font;
	static unsigned int s_fontSize;

	sf::Vector2i m_position;
	bool m_selected = false;
	bool m_active = false;

protected:
	static const sf::Vector2i& TileSize;
};

}
