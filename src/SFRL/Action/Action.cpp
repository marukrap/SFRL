#include "Action/Action.hpp"
#include "StateStack.hpp"

namespace rl
{

World* Action::s_world = nullptr;
StateStack* Action::s_stack = nullptr;

bool Action::isDone() const
{
	return true;
}

void Action::setWorld(World& world)
{
	s_world = &world;
}

void Action::setStateStack(StateStack& stack)
{
	s_stack = &stack;
}

World& Action::getWorld() const
{
	return *s_world;
}

void Action::popState()
{
	s_stack->popState();
}

void Action::clearStates()
{
	s_stack->clearStates();
}

void Action::pushState(State::Ptr state)
{
	s_stack->pushState(std::move(state));
}

}
