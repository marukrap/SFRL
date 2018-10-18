#pragma once

#include "State.hpp"

#include <vector>

namespace rl
{

class StateStack : public sf::Drawable
{
public:
	StateStack() = default;

	StateStack(const StateStack&) = delete;
	StateStack& operator=(const StateStack&) = delete;

	bool isEmpty() const;

	void pushState(State::Ptr state);
	void popState();
	void clearStates();

	void handleEvent(const sf::Event& event);
	void update(sf::Time dt);

private:
	enum class Action
	{
		Push,
		Pop,
		Clear,
	};

	void applyPendingChanges();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<State::Ptr> m_stack;
	std::vector<std::pair<Action, State::Ptr>> m_pendingList;
};

}
