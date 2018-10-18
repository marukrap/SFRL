#include "Map/Dijkstra.hpp"

#include <iostream>

namespace rl
{

void Dijkstra::setMap(Map& map)
{
	m_map = &map;
	m_costs.resize(map.width * map.height);
}

Direction Dijkstra::getNextDirection(const sf::Vector2i& position) const
{
	int lowestCost = getCost(position);
	Direction nextDir;

	for (const auto& dir : Direction::All)
	{
		const sf::Vector2i next = position + dir;

		if (!m_map->isInBounds(next))
			continue;

		const int cost = getCost(next);

		if (cost < lowestCost)
		{
			lowestCost = cost;
			nextDir = dir;
		}
	}

	return nextDir;
}

Direction Dijkstra::getHighestNextDirection(const sf::Vector2i& position) const
{
	int highestCost = getCost(position);
	Direction nextDir;

	for (const auto& dir : Direction::All)
	{
		const sf::Vector2i next = position + dir;

		if (!m_map->isInBounds(next))
			continue;

		const int cost = getCost(next);

		if (cost > highestCost)
		{
			highestCost = cost;
			nextDir = dir;
		}
	}

	return nextDir;
}

std::vector<sf::Vector2i> Dijkstra::getPath(const sf::Vector2i& start, const sf::Vector2i& goal) const
{
	// TODO: remove 'start' variable

	std::vector<sf::Vector2i> path;
	sf::Vector2i current = goal;

	while (true)
	{
		path.emplace_back(current);

		if (current == start)
			break;

		const Direction nextDir = getNextDirection(current);

		if (nextDir == Direction::None)
			break;

		current += nextDir;
	}

	std::reverse(path.begin(), path.end());

	return path;
}

void Dijkstra::print() const
{
	std::cout << '\n';

	for (int y = 0; y < m_map->height; ++y)
	{
		for (int x = 0; x < m_map->width; ++x)
		{
			char ch = '\0';
			int cost = getCost({ x, y });

			if (cost == IntMax)
				ch = ' ';

			else
			{
				cost = cost % 62;

				if (cost < 10)
					ch = '0' + cost;
				else if (cost < 36)
					ch = 'a' + cost - 10;
				else // if (cost < 62)
					ch = 'A' + cost - 36;
			}

			std::cout << ch;
		}

		std::cout << '\n';
	}
}

#if 0

void DijkstraOld::setMap(Map& map)
{
	m_map = &map;
	m_costs.resize(map.width * map.height);
}

#endif

}
