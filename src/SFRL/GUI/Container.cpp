#include "GUI/Container.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include <cassert>

namespace rl
{

Container::Container()
{
	assignKeys(Action::Previous, { sf::Keyboard::Up, sf::Keyboard::Numpad8 });
	assignKeys(Action::Next,     { sf::Keyboard::Down, sf::Keyboard::Numpad2 });
	assignKeys(Action::Activate, { sf::Keyboard::Enter, sf::Keyboard::Space });
}

std::vector<sf::Keyboard::Key> Container::getAssignedKeys(Action action) const
{
	std::vector<sf::Keyboard::Key> keys;

	for (const auto& pair : m_keyBindings)
	{
		if (pair.second == action)
			keys.emplace_back(pair.first);
	}

	return keys;
}

void Container::assignKeys(Action action, const std::vector<sf::Keyboard::Key>& keys)
{
	for (auto it = m_keyBindings.begin(); it != m_keyBindings.end(); )
	{
		if (it->second == action)
			it = m_keyBindings.erase(it);
		else
			++it;
	}

	for (const auto& key : keys)
		m_keyBindings[key] = action;
}

void Container::clear()
{
	m_selectedChild = -1;
	m_children.clear();
}

void Container::pack(Component::Ptr component)
{
	m_children.emplace_back(std::move(component));

	if (!hasSelection() && m_children.back()->isSelectable())
		select(m_children.size() - 1);
}

void Container::handleEvent(const sf::Event& event)
{
	if (hasSelection() && m_children[m_selectedChild]->isActive())
		m_children[m_selectedChild]->handleEvent(event);

	else if (event.type == sf::Event::KeyPressed)
	{
		const auto found = m_keyBindings.find(event.key.code);

		if (found != m_keyBindings.end())
		{
			switch (found->second)
			{
			case Action::Previous:
				selectPrevious();
				break;

			case Action::Next:
				selectNext();
				break;

			case Action::Activate:
				if (hasSelection())
					m_children[m_selectedChild]->activate();
				break;
			}
		}
	}

	else if (event.type == sf::Event::MouseMoved)
	{
		const float x = static_cast<float>(event.mouseMove.x);
		const float y = static_cast<float>(event.mouseMove.y);

		const sf::Vector2f point = getTransform().getInverse().transformPoint(x, y);

		for (std::size_t i = 0; i < m_children.size(); ++i)
		{
			if (m_children[i]->contains(point))
			{
				select(i);
				break;
			}
		}
	}

	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		const float x = static_cast<float>(event.mouseButton.x);
		const float y = static_cast<float>(event.mouseButton.y);

		const sf::Vector2f point = getTransform().getInverse().transformPoint(x, y);

		for (std::size_t i = 0; i < m_children.size(); ++i)
		{
			if (m_children[i]->contains(point))
			{
				select(i);
				m_children[i]->activate();
				break;
			}
		}
	}
}

void Container::update(sf::Time dt)
{
	for (auto& child : m_children)
		child->update(dt);
}

int Container::getSelectedChild() const
{
	return m_selectedChild;
}

bool Container::hasSelection() const
{
	return m_selectedChild >= 0;
}

void Container::select(std::size_t index)
{
	assert(m_children[index]->isSelectable());

	if (hasSelection())
		m_children[m_selectedChild]->deselect();

	m_children[index]->select();
	m_selectedChild = index;
}

void Container::selectPrevious()
{
	if (!hasSelection())
		return;

	int prev = m_selectedChild;
	do
		prev = (prev + m_children.size() - 1) % m_children.size();
	while (!m_children[prev]->isSelectable());

	select(prev);
}

void Container::selectNext()
{
	if (!hasSelection())
		return;

	int next = m_selectedChild;
	do
		next = (next + 1) % m_children.size();
	while (!m_children[next]->isSelectable());

	select(next);
}

void Container::onActivate()
{
	if (hasSelection())
		m_children[m_selectedChild]->select();
}

void Container::onDeactivate()
{
	if (hasSelection())
		m_children[m_selectedChild]->deselect();
}

void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (const auto& child : m_children)
		target.draw(*child, states);
}

}
