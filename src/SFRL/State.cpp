#include "State.hpp"
#include "StateStack.hpp"

namespace rl
{

void State::setStateStack(StateStack& stack)
{
	m_stack = &stack;
}

void State::popState()
{
	m_stack->popState();
}

void State::clearStates()
{
	m_stack->clearStates();
}

void State::pushState(State::Ptr state)
{
	m_stack->pushState(std::move(state));
}

}
