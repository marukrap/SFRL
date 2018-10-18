#include "Map/AStar.hpp"
#include "Map/Map.hpp"
#include "Direction.hpp"

#include <queue>
#include <unordered_map>

namespace std
{
	template <>
	struct hash<sf::Vector2i>
	{
		inline std::size_t operator()(const sf::Vector2i& location) const
		{
			return location.x * 1812433253 + location.y;
		}
	};
}

namespace rl::astar
{

int heuristic(const sf::Vector2i& from, const sf::Vector2i& to)
{
	return std::abs(to.x - from.x) + std::abs(to.y - from.y);
}

int cost(const Map& map, const sf::Vector2i& from, const sf::Vector2i& to)
{
	// UNDONE:
	return 1;
}

std::vector<sf::Vector2i> search(const Map& map, const sf::Vector2i& start, const sf::Vector2i& goal)
{
	using Element = std::pair<sf::Vector2i, int>;

	struct Greater
	{
		inline bool operator()(const Element& lhs, const Element& rhs) const
		{
			if (lhs.second == rhs.second)
			{
				if (lhs.first.y == rhs.first.y)
					return lhs.first.x > rhs.first.x;

				return lhs.first.y > rhs.first.y;
			}

			return lhs.second > rhs.second;
		}
	};

	// TODO: bucket queue?
	std::priority_queue<Element, std::vector<Element>, Greater> frontier;
	std::unordered_map<sf::Vector2i, sf::Vector2i> cameFrom;
	std::unordered_map<sf::Vector2i, int> costSoFar;

	frontier.emplace(start, 0);
	cameFrom[start] = start;
	costSoFar[start] = 0;

	std::vector<sf::Vector2i> path;

	while (!frontier.empty())
	{
		sf::Vector2i current = frontier.top().first;
		frontier.pop();

		if (current == goal)
		{
			// construct path

			while (current != start)
			{
				path.emplace_back(current);
				current = cameFrom[current];
			}

			path.emplace_back(start); // optional
			std::reverse(path.begin(), path.end());

			break;
		}

		for (const auto& dir : Direction::All)
		{
			const sf::Vector2i next = current + dir;

			if (!map.isInBounds(next) || !map.at(next).passable)
				continue;

			const auto found = costSoFar.find(next);
			const int newCost = costSoFar[current] + cost(map, current, next);

			if (found == costSoFar.end() || newCost < found->second)
			{
				frontier.emplace(next, newCost + heuristic(next, goal));
				cameFrom[next] = current;
				costSoFar[next] = newCost;
			}
		}
	}

	return path;
}

}
