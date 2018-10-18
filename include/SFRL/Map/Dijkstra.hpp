#pragma once

// credit: http://www.roguebasin.com/index.php?title=The_Incredible_Power_of_Dijkstra_Maps

/*
 *  NOTE: to improve debug mode performance in Visual Studio,
 *        1. setting _ITERATOR_DEBUG_LEVEL=0 // need to recompile SFML
 *        2. replacing /RTC1 with /RTCu
 *        3. inlining with /Ob1
 */

#include "Map.hpp"
#include "../Direction.hpp"

#include <queue>
// #include <set>

// TODO: remove macro
#define DIRECTIONS Direction::Cardinal // cardinal or all

namespace rl
{

// priority queue
class Dijkstra
{
public:
	using Element = std::pair<sf::Vector2i, int>;

	struct Greater
	{
		bool operator()(const Element& lhs, const Element& rhs) const;
	};

public:
	Dijkstra() = default;

	Dijkstra(const Dijkstra&) = delete;
	Dijkstra& operator=(const Dijkstra&) = delete;

	void setMap(Map& map);

	int getCost(const sf::Vector2i& position) const;
	void addCost(const sf::Vector2i& position, int cost);

	Direction getNextDirection(const sf::Vector2i& position) const;
	Direction getHighestNextDirection(const sf::Vector2i& position) const;
	std::vector<sf::Vector2i> getPath(const sf::Vector2i& start, const sf::Vector2i& goal) const;

	void clear();

	void compute();
	void computeVisible();
	void computeExplored();
	void computeSafetyMap(const Dijkstra& dijkstra);

	void print() const;

private:
	static constexpr int IntMax = std::numeric_limits<int>::max() - 1;

	Map* m_map = nullptr;
	std::vector<int> m_costs;
	std::priority_queue<Element, std::vector<Element>, Greater> m_frontier;
};

#if 0

// set
class DijkstraOld
{
public:
	using Element = std::pair<sf::Vector2i, int>;

	struct Less
	{
		bool operator()(const Element& lhs, const Element& rhs) const;
	};

public:
	DijkstraOld() = default;

	DijkstraOld(const DijkstraOld&) = delete;
	DijkstraOld& operator=(const DijkstraOld&) = delete;

	void setMap(Map& map);

	int getCost(const sf::Vector2i& position) const;
	void addCost(const sf::Vector2i& position, int cost);

	void clear();

	void compute();
	void computeSafetyMap(const DijkstraOld& dijkstra);

private:
	static constexpr int IntMax = std::numeric_limits<int>::max() - 1;

	Map* m_map = nullptr;
	std::vector<int> m_costs;
	std::set<Element, Less> m_actives;
};

#endif

}

#include "Dijkstra.inl"
