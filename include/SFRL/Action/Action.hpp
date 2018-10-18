#pragma once

#include "../State.hpp"

// TODO: remove Actor, World dependencies
class Actor;
class World;

namespace rl
{

// class StateStack;

class Action
{
public:
	using Ptr = std::unique_ptr<Action>;

public:
	Action() = default;
	virtual ~Action() = default;

	Action(const Action&) = delete;
	Action& operator=(const Action&) = delete;

	virtual bool isDone() const;
	virtual bool perform(Actor& actor) = 0;

	static void setWorld(World& world);
	static void setStateStack(StateStack& stack);

protected:
	World& getWorld() const;

	template <typename T, typename... Args>
	T* pushState(Args&&... args);

	void popState();
	void clearStates();

private:
	void pushState(State::Ptr state);

private:
	static World* s_world;
	static StateStack* s_stack;
};

template <typename T, typename... Args>
T* Action::pushState(Args&&... args)
{
	auto state = std::make_unique<T>(std::forward<Args>(args)...);

	T* result = state.get();
	pushState(std::move(state));

	return result;
}

}
