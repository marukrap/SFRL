#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <cassert>

// HACK: ResourceManager
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
//

/*
namespace sf
{
	class Texture;
	class Font;
	class SoundBuffer;
}
*/

namespace rl
{

template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
	void load(const Identifier& id, const std::string& filename);
	void load(const Identifier& id, const void* data, std::size_t size);

	Resource& get(const Identifier& id);
	const Resource& get(const Identifier& id) const;

	void insertResource(const Identifier& id, std::unique_ptr<Resource> resource);

private:
	std::unordered_map<Identifier, std::unique_ptr<Resource>> m_resourceMap;
};

// HACK: ResourceManager
struct ResourceManager
{
	ResourceHolder<sf::Texture, std::string> textures;
	ResourceHolder<sf::Font, std::string> fonts;
	ResourceHolder<sf::SoundBuffer, std::string> soundBuffers;
};
//

}

#include "ResourceManager.inl"
