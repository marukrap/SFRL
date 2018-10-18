#pragma once

// TODO: custom iterator

#include "Component.hpp"

#include <SFML/Window/Keyboard.hpp>

#include <unordered_map>

namespace rl
{

class Container : public Component
{
public:
	using Ptr = std::unique_ptr<Container>;

	enum class Action
	{
		Previous,
		Next,
		Activate,
	};

public:
	Container();

	std::vector<sf::Keyboard::Key> getAssignedKeys(Action action) const;
	void assignKeys(Action action, const std::vector<sf::Keyboard::Key>& keys);

	void clear();
	// void pack(Component::Ptr component);

	template <typename T, typename... Args>
	T* create(Args&&... args);

	void handleEvent(const sf::Event& event) override;
	void update(sf::Time dt) override;

	int getSelectedChild() const;

	void select(std::size_t index);

private:
	void pack(Component::Ptr component);

	bool hasSelection() const;

	// void select(std::size_t index);
	void selectPrevious();
	void selectNext();

	void onActivate() override;
	void onDeactivate() override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	int m_selectedChild = -1;
	std::vector<Component::Ptr> m_children;
	std::unordered_map<sf::Keyboard::Key, Action> m_keyBindings;
};

template <typename T, typename... Args>
T* Container::create(Args&&... args)
{
	auto component = std::make_unique<T>(std::forward<Args>(args)...);

	T* result = component.get();
	pack(std::move(component));

	return result;
}

}
