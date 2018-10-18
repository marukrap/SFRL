#include "GUI/Component.hpp"

namespace rl
{

sf::Vector2i Component::s_tileSize(1, 1);
const sf::Font* Component::s_font = nullptr;
unsigned int Component::s_fontSize = 0;

const sf::Vector2i& Component::TileSize = s_tileSize;

const sf::Vector2i& Component::getPosition() const
{
	return m_position;
}

void Component::setPosition(int x, int y)
{
	m_position.x = x;
	m_position.y = y;

	const float fx = static_cast<float>(x * s_tileSize.x);
	const float fy = static_cast<float>(y * s_tileSize.y);

	sf::Transformable::setPosition(fx, fy);
}

void Component::setPosition(const sf::Vector2i& position)
{
	setPosition(position.x, position.y);
}

void Component::move(int x, int y)
{
	setPosition(m_position.x + x, m_position.y + y);
}

void Component::move(const sf::Vector2i& offset)
{
	move(offset.x, offset.y);
}

bool Component::isSelectable() const
{
	return false;
}

bool Component::isSelected() const
{
	return m_selected;
}

void Component::select()
{
	m_selected = true;

	onSelect();
}

void Component::deselect()
{
	m_selected = false;

	onDeselect();
}

bool Component::isActive() const
{
	return m_active;
}

void Component::activate()
{
	m_active = true;

	onActivate();
}

void Component::deactivate()
{
	m_active = false;

	onDeactivate();
}

bool Component::contains(const sf::Vector2f& point) const
{
	return false;
}

void Component::handleEvent(const sf::Event& event)
{
}

void Component::update(sf::Time dt)
{
}

void Component::setTileSize(const sf::Vector2i& tileSize)
{
	s_tileSize = tileSize;
}

void Component::setFont(const sf::Font& font)
{
	s_font = &font;
}

void Component::setFontSize(unsigned int fontSize)
{
	s_fontSize = fontSize;
}

const sf::Font* Component::getFont() const
{
	return s_font;
}

unsigned int Component::getFontSize() const
{
	return s_fontSize;
}

void Component::onSelect()
{
}

void Component::onDeselect()
{
}

void Component::onActivate()
{
}

void Component::onDeactivate()
{
}

}
