namespace rl
{

template <typename Entity>
std::size_t Level<Entity>::s_idCounter = 0;

template <typename Entity>
Level<Entity>::Level(const sf::Vector2i& size)
	: Map(size)
	, m_layers(s_idCounter)
{
}

template <typename Entity>
template <typename T>
std::size_t Level<Entity>::getId()
{
	static std::size_t id = s_idCounter++;

	return id;
}

template <typename Entity>
template <typename T>
void Level<Entity>::addLayer()
{
	const std::size_t id = getId<T>();
}

template <typename Entity>
template <typename T, typename... Args>
T* Level<Entity>::create(Args&&... args)
{
	auto entity = std::make_unique<T>(std::forward<Args>(args)...);

	T* result = entity.get();
	attachBack<T>(std::move(entity));

	return result;
}

template <typename Entity>
template <typename T>
void Level<Entity>::attachBack(std::unique_ptr<T> entity)
{
	const std::size_t id = getId<T>();

	m_layers[id].emplace_back(std::move(entity));
}

template <typename Entity>
template <typename T>
void Level<Entity>::attachFront(std::unique_ptr<T> entity)
{
	const std::size_t id = getId<T>();

	m_layers[id].insert(m_layers[id].begin(), std::move(entity));
}

template <typename Entity>
template <typename T>
std::unique_ptr<T> Level<Entity>::detach(Entity& entity)
{
	const std::size_t id = getId<T>();
	EntityLayer& layer = m_layers[id];

	const auto found = std::find_if(layer.begin(), layer.end(),
		[&] (const auto& e) { return e.get() == &entity; });
	assert(found != layer.end());

	// auto result = std::move(*found);
	auto result = std::unique_ptr<T>(static_cast<T*>(found->release()));
	layer.erase(found);

	return result;
}

template <typename Entity>
template <typename T>
bool Level<Entity>::isEmpty() const
{
	const std::size_t id = getId<T>();

	return m_layers[id].empty();
}

template <typename Entity>
template <typename T>
T* Level<Entity>::get(const sf::Vector2i& position) const
{
	const std::size_t id = getId<T>();
	const EntityLayer& layer = m_layers[id];

	const auto found = std::find_if(layer.begin(), layer.end(),
		[&] (const auto& e) { return !e->isDestroyed() && e->getPosition() == position; });

	if (found == layer.end())
		return nullptr;

	return static_cast<T*>(found->get());
}

template <typename Entity>
template <typename T>
T* Level<Entity>::getClosest(const sf::Vector2i& position) const
{
	const std::size_t id = getId<T>();
	const EntityLayer& layer = m_layers[id];

	int closestDistance = std::numeric_limits<int>::max();
	Entity* closestEntity = nullptr;

	for (const auto& entity : layer)
	{
		// HACK: exclude invisible entity
		if (entity->isDestroyed() || !entity->isVisible())
			continue;

		const int distance = lengthSquared(entity->getPosition() - position);

		// HACK: exclude distance 0 (the observer)
		if (closestDistance > distance && distance != 0)
		{
			closestDistance = distance;
			closestEntity = entity.get();
		}
	}

	return static_cast<T*>(closestEntity);
}

template <typename Entity>
template <typename T>
std::vector<T*> Level<Entity>::getAll() const
{
	const std::size_t id = getId<T>();
	const EntityLayer& layer = m_layers[id];

	std::vector<T*> results;

	for (const auto& entity : layer)
	{
		if (entity->isDestroyed())
			continue;

		results.emplace_back(static_cast<T*>(entity.get()));
	}

	return results;
}

template <typename Entity>
template <typename T>
std::vector<T*> Level<Entity>::getAll(std::function<bool(const T&)> condition) const
{
	const std::size_t id = getId<T>();
	const EntityLayer& layer = m_layers[id];

	std::vector<T*> results;

	for (const auto& entity : layer)
	{
		if (entity->isDestroyed())
			continue;

		T* result = static_cast<T*>(entity.get());

		if (condition(*result))
			results.emplace_back(result);
	}

	return results;
}

template <typename Entity>
void Level<Entity>::removeWrecks()
{
	for (auto& layer : m_layers)
		layer.erase(std::remove_if(layer.begin(), layer.end(),
			std::mem_fn(&Entity::isMarkedForRemoval)), layer.end());
}

}
