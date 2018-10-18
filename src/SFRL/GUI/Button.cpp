#include "GUI/Button.hpp"
#include "Utility.hpp"
// #include "Console.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace rl
{

sf::Color Button::s_selectedColor(94, 166, 50, 128);
sf::Color Button::s_pressedColor(255, 61, 61, 128);

Button::Button(const sf::Vector2i& size)
	: m_size(size)
{
	const float width = static_cast<float>(m_size.x * TileSize.x);
	const float height = static_cast<float>(m_size.y * TileSize.y);

	m_button.setSize({ width, height });
	m_button.setFillColor(sf::Color::Transparent);
}

Button::Button(const sf::Font& font, int fontSize, const sf::Vector2i& size)
	: Button(size)
{
	m_text.setFont(font);
	m_text.setCharacterSize(fontSize);
	m_text.setPosition(std::floor(m_button.getSize().x / 2), std::floor(m_button.getSize().y / 2));
}

Button::Button()
	: Button(*getFont(), getFontSize())
{
}

const sf::Vector2i& Button::getSize() const
{
	return m_size;
}

void Button::setSize(const sf::Vector2i& size)
{
	m_size = size;

	const float width = static_cast<float>(m_size.x * TileSize.x);
	const float height = static_cast<float>(m_size.y * TileSize.y);

	m_button.setSize({ width, height });
	setTextAlignment(m_alignment);
}

void Button::setText(const sf::String& string)
{
	m_text.setString(string);

	// auto resize
	if (m_size == sf::Vector2i(0, 0))
	{
		const sf::FloatRect bounds = m_text.getLocalBounds();

		m_size.x = static_cast<int>(std::ceil((bounds.left + bounds.width) / TileSize.x + 1.5f));
		m_size.y = static_cast<int>(std::ceil((bounds.top + bounds.height) / TileSize.y));

		setSize(m_size);
	}

	else
		setTextAlignment(m_alignment);
}

void Button::setTextColor(const sf::Color& color)
{
	m_text.setFillColor(color);
}

void Button::setTextAlignment(Alignment align)
{
	m_alignment = align;

	// apply alignment
	if (m_alignment == Alignment::Centered)
	{
		centerOriginA(m_text);
		m_text.setPosition(std::floor(m_button.getSize().x / 2), std::floor(m_button.getSize().y / 2));
	}

	else if (m_alignment == Alignment::Left)
	{
		setAnchorA(m_text, { 0.f, 0.5f });
		m_text.setPosition(1.f * TileSize.x, std::floor(m_button.getSize().y / 2));
	}

	else // Alignment::Right
	{
		setAnchorA(m_text, { 1.f, 0.5f });
		m_text.setPosition((m_size.x - 1.f) * TileSize.x, std::floor(m_button.getSize().y / 2));
	}
}

void Button::setCallback(Callback callback)
{
	m_callback = std::move(callback);
}

void Button::setToggle(bool flag)
{
	m_toggle = flag;
}

bool Button::isSelectable() const
{
	return m_selectable;
}

void Button::setSelectable(bool flag)
{
	m_selectable = flag;
}

bool Button::contains(const sf::Vector2f& point) const
{
	if (!m_selectable)
		return false;

	return getTransform().transformRect(m_button.getGlobalBounds()).contains(point);
}

void Button::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
			deactivate();
	}
}

void Button::update(sf::Time dt)
{
	if (!isSelected() && m_button.getFillColor().a > 0)
	{
		sf::Color color = m_button.getFillColor();
		color.a = std::max(0, color.a - dt.asMilliseconds()); // 16
		m_button.setFillColor(color);
	}
}

void Button::setButtonColors(const sf::Color& selectedColor, const sf::Color& pressedColor)
{
	s_selectedColor = selectedColor;
	s_pressedColor = pressedColor;
}

void Button::changeButton(Type buttonType)
{
	switch (buttonType)
	{
	case Type::Normal:
		// m_button.setFillColor(sf::Color::Transparent);
		break;

	case Type::Selected:
		m_button.setFillColor(s_selectedColor);
		break;

	case Type::Pressed:
		m_button.setFillColor(s_pressedColor);
		break;
	}
}

void Button::onSelect()
{
	changeButton(Type::Selected);
}

void Button::onDeselect()
{
	if (isActive())
		changeButton(Type::Pressed);
	else
		changeButton(Type::Normal);
}

void Button::onActivate()
{
	if (m_toggle)
		changeButton(Type::Pressed);

	if (m_callback)
		m_callback();

	if (!m_toggle)
		deactivate();

	// m_sounds.play(SoundID::Button);
}

void Button::onDeactivate()
{
	if (m_toggle)
	{
		if (isSelected())
			changeButton(Type::Selected);
		else
			changeButton(Type::Normal);
	}
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_button, states);
	target.draw(m_text, states);
}

/*
void Button::draw(Console& console)
{
	const int length = static_cast<int>(m_text.getString().getSize());
	sf::Vector2i pos = getPosition();

	if (m_alignment == Alignment::Left)
		pos.x += 1;
	else if (m_alignment == Alignment::Centered)
		pos.x += (m_size.x - length) / 2;
	else // if (m_alignment == Alignment::Right)
		pos.x += m_size.x - length - 1;

	console.setString(pos.x, pos.y, m_text.getString(), m_text.getFillColor());
	console.setColor(pos.x, pos.y, m_size.x, m_size.y, m_button.getFillColor());
}
*/

}
