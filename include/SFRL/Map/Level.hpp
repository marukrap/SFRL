#pragma once

#include "Map.hpp"
#include "../Utility.hpp"

#include <memory>
#include <functional>
#include <cassert>

namespace rl
{

template <typename Entity>
class Level : public Map
{
public:
	using Ptr = std::unique_ptr<Level>;
	using EntityLayer = std::vector<std::unique_ptr<Entity>>;

public:
	explicit Level(const sf::Vector2i& size = { 0, 0 });

	template <typename T>
	static std::size_t getId();
	template <typename T>
	static void addLayer();

	template <typename T, typename... Args>
	T* create(Args&&... args);

	template <typename T>
	void attachBack(std::unique_ptr<T> entity);
	template <typename T>
	void attachFront(std::unique_ptr<T> entity);
	template <typename T>
	std::unique_ptr<T> detach(Entity& entity);

	template <typename T>
	bool isEmpty() const;
	template <typename T>
	T* get(const sf::Vector2i& position) const;
	template <typename T>
	T* getClosest(const sf::Vector2i& position) const;
	template <typename T>
	std::vector<T*> getAll() const;
	template <typename T>
	std::vector<T*> getAll(std::function<bool(const T&)> condition) const;

	// std::vector<Entity*> getAll(const sf::Vector2i& position);
	// std::vector<Entity*> getClosestAll(const sf::Vector2i& position);

	void removeWrecks();

private:
	static std::size_t s_idCounter;

protected:
	std::vector<EntityLayer> m_layers;
};

}

#include "Level.inl"
