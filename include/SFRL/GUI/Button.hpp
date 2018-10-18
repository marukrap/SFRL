#pragma once

#include "Component.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <functional>

namespace rl
{

// class Console;

class Button : public Component
{
public:
	using Ptr = std::unique_ptr<Button>;
	using Callback = std::function<void()>;

	enum class Type
	{
		Normal,
		Selected,
		Pressed,
	};

public:
	explicit Button(const sf::Vector2i& size);
	Button(const sf::Font& font, int fontSize, const sf::Vector2i& size = { 0, 0 });
	Button();

	const sf::Vector2i& getSize() const;
	void setSize(const sf::Vector2i& size);

	void setText(const sf::String& string);
	void setTextColor(const sf::Color& color);
	void setTextAlignment(Alignment align);

	void setCallback(Callback callback);
	void setToggle(bool flag);

	bool isSelectable() const override;
	void setSelectable(bool flag);

	bool contains(const sf::Vector2f& point) const override;
	void handleEvent(const sf::Event& event) override;
	void update(sf::Time dt) override;

	static void setButtonColors(const sf::Color& selectedColor, const sf::Color& pressedColor);

private:
	void changeButton(Type buttonType);

	void onSelect() override;
	void onDeselect() override;

	void onActivate() override;
	void onDeactivate() override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	// void draw(Console& console);

private:
	static sf::Color s_selectedColor;
	static sf::Color s_pressedColor;

	Callback m_callback;
	sf::Vector2i m_size;
	sf::RectangleShape m_button;
	sf::Text m_text;
	bool m_toggle = false;
	bool m_selectable = true;
	Alignment m_alignment = Alignment::Centered;
	// SoundPlayer& m_sounds;
};

}
