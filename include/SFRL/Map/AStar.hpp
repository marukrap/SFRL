#pragma once

#include <SFML/System/Vector2.hpp>

#include <vector>

namespace rl
{
	class Map;
}

namespace rl::astar
{

// int heuristic(const sf::Vector2i& from, const sf::Vector2i& to);
// int cost(const Map& map, const sf::Vector2i& from, const sf::Vector2i& to);
std::vector<sf::Vector2i> search(const Map& map, const sf::Vector2i& start, const sf::Vector2i& goal);

}
