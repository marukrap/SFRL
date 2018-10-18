#include "StateStack.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace rl
{

bool StateStack::isEmpty() const
{
	return m_stack.empty();
}

void StateStack::pushState(State::Ptr state)
{
	state->setStateStack(*this);
	m_pendingList.emplace_back(Action::Push, std::move(state));
}

void StateStack::popState()
{
	m_pendingList.emplace_back(Action::Pop, nullptr);
}

void StateStack::clearStates()
{
	m_pendingList.emplace_back(Action::Clear, nullptr);
}

void StateStack::handleEvent(const sf::Event& event)
{
	for (auto it = m_stack.rbegin(); it != m_stack.rend(); ++it)
	{
		if (!(*it)->handleEvent(event))
			break;
	}

	applyPendingChanges();
}

void StateStack::update(sf::Time dt)
{
	for (auto it = m_stack.rbegin(); it != m_stack.rend(); ++it)
	{
		if (!(*it)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::applyPendingChanges()
{
	for (auto& [action, state] : m_pendingList)
	{
		switch (action)
		{
		case Action::Push:
			m_stack.emplace_back(std::move(state));
			break;

		case Action::Pop:
			m_stack.pop_back();
			break;

		case Action::Clear:
			m_stack.clear();
			break;
		}
	}

	m_pendingList.clear();
}

void StateStack::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& state : m_stack)
		target.draw(*state, states);
}

}
