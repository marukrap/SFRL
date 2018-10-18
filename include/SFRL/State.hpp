#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

#include <memory>

namespace sf
{
	class Event;
}

namespace rl
{

class StateStack;

class State : public sf::Drawable
{
public:
	using Ptr = std::unique_ptr<State>;

public:
	State() = default;
	virtual ~State() = default;

	State(const State&) = delete;
	State& operator=(const State&) = delete;

	virtual bool handleEvent(const sf::Event& event) = 0;
	virtual bool update(sf::Time dt) = 0;

	void setStateStack(StateStack& stack);

protected:
	template <typename T, typename... Args>
	T* pushState(Args&&... args);

	void popState();
	void clearStates();

private:
	void pushState(State::Ptr state);

private:
	StateStack* m_stack = nullptr;
};

template <typename T, typename... Args>
T* State::pushState(Args&&... args)
{
	auto state = std::make_unique<T>(std::forward<Args>(args)...);

	T* result = state.get();
	pushState(std::move(state));

	return result;
}

}
