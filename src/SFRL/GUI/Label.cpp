#include "GUI/Label.hpp"
#include "Utility.hpp"
// #include "Console.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace rl
{

/*
Label::Label(const sf::String& string)
	: m_alignment(Alignment::Left)
{
	m_text.setString(string);
}
*/

Label::Label(const sf::Font& font, int fontSize, const sf::Vector2i& size)
	: m_font(&font)
	, m_fontSize(fontSize)
	, m_size(size)
{
}

Label::Label()
	: Label(*getFont(), getFontSize())
{
}

const sf::Vector2i& Label::getSize() const
{
	return m_size;
}

void Label::setSize(const sf::Vector2i& size)
{
	m_size = size;
	m_textsNeedUpdate = true;
}

std::size_t Label::getNumLines() const
{
	if (m_textsNeedUpdate)
		updateTexts();

	return m_texts.size();
}

void Label::setText(const sf::String& string)
{
	m_string = string;
	m_textsNeedUpdate = true;

	// auto resize
	if (m_size == sf::Vector2i(0, 0))
	{
		const sf::FloatRect bounds = sf::Text(m_string, *m_font, m_fontSize).getLocalBounds();

		m_size.x = static_cast<int>(std::ceil((bounds.left + bounds.width) / TileSize.x + 1.5f));
		m_size.y = static_cast<int>(std::ceil((bounds.top + bounds.height) / TileSize.y));
	}
}

void Label::setTextColor(const sf::Color& color)
{
	m_color = color;
	m_textsNeedUpdate = true;
}

void Label::setTextWrap(bool flag)
{
	m_wrap = flag;
	m_textsNeedUpdate = true;
}

void Label::setTextAlignment(Alignment align)
{
	m_alignment = align;
	m_textsNeedUpdate = true;
}

void Label::updateTexts() const
{
	m_texts.clear();

	const float width = static_cast<float>(m_size.x * TileSize.x);
	const float height = static_cast<float>(m_size.y * TileSize.y);

	if (!m_wrap)
	{
		m_texts.emplace_back(m_string, *m_font, m_fontSize);
		m_texts.back().setPosition(std::floor(width / 2), std::floor(height / 2));
	}

	else
	{
		// TODO: scrollbar

		const float whitespaceWidth = m_font->getGlyph(L' ', m_fontSize, false).advance;

		std::wstring curStr = m_string;

		while (!curStr.empty())
		{
			float x = 0.f;
			wchar_t prevChar = 0;
			std::size_t i = 0;
			std::size_t lastSpace = 0;

			for (; i < curStr.size(); ++i)
			{
				const wchar_t curChar = curStr[i];

				x += m_font->getKerning(prevChar, curChar, m_fontSize);
				prevChar = curChar;

				if (curChar == L' ')
				{
					x += whitespaceWidth;
					lastSpace = i;
					continue;
				}

				else if (curChar == L'\t')
				{
					x += whitespaceWidth * 4;
					lastSpace = i;
					continue;
				}

				else if (curChar == L'\n')
				{
					lastSpace = i;
					break;
				}

				const sf::Glyph& glyph = m_font->getGlyph(curChar, m_fontSize, false);

				// if (x + glyph.bounds.left + glyph.bounds.width > width)
				if (x + glyph.bounds.width > width)
					break;

				x += glyph.advance;
			}

			if (i != curStr.size() && lastSpace > 0)
				i = lastSpace;

			m_texts.emplace_back(curStr.substr(0, i), *m_font, m_fontSize);

			if (curStr[i] == L' ' || curStr[i] == L'\t' || curStr[i] == L'\n')
				++i;

			curStr = curStr.substr(i);
		}
	}

	// apply color and alignment
	for (std::size_t i = 0; i < m_texts.size(); ++i)
	{
		m_texts[i].setFillColor(m_color);

		if (m_alignment == Alignment::Centered)
		{
			centerOriginA(m_texts[i]);
			m_texts[i].setPosition(std::floor(m_size.x * TileSize.x / 2.f), std::floor((i + 0.5f) * TileSize.y));
		}

		else if (m_alignment == Alignment::Left)
		{
			setAnchorA(m_texts[i], { 0.f, 0.5f });
			m_texts[i].setPosition(0.f, std::floor((i + 0.5f) * TileSize.y));
		}

		else // Alignment::Right
		{
			setAnchorA(m_texts[i], { 1.f, 0.5f });
			m_texts[i].setPosition(static_cast<float>(m_size.x * TileSize.x), std::floor((i + 0.5f) * TileSize.y));
		}
	}

	m_textsNeedUpdate = false;
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_textsNeedUpdate)
		updateTexts();

	states.transform *= getTransform();

	for (const auto& text : m_texts)
		target.draw(text, states);
}

/*
void Label::draw(Console& console)
{
	const int length = static_cast<int>(m_text.getString().getSize());
	sf::Vector2i pos = getPosition();

	if (m_alignment == Alignment::Centered)
		pos.x += (m_size.x - length) / 2;
	else if (m_alignment == Alignment::Right)
		pos.x += m_size.x - length;

	console.setString(pos.x, pos.y, m_text.getString(), m_text.getFillColor());
}
*/

}
