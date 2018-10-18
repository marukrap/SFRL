namespace rl
{

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(const Identifier& id, const std::string& filename)
{
	auto resource = std::make_unique<Resource>();

	if (!resource->loadFromFile(filename))
		throw std::runtime_error("Failed to load resource \"" + filename + "\".");

	insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(const Identifier& id, const void* data, std::size_t size)
{
	auto resource = std::make_unique<Resource>();

	if (!resource->loadFromMemory(data, size))
		throw std::runtime_error("Failed to load resource from memory.");

	insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(const Identifier& id)
{
	const auto found = m_resourceMap.find(id);
	assert(found != m_resourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(const Identifier& id) const
{
	const auto found = m_resourceMap.find(id);
	assert(found != m_resourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(const Identifier& id, std::unique_ptr<Resource> resource)
{
	const auto inserted = m_resourceMap.emplace(id, std::move(resource));
	assert(inserted.second);
}

}
