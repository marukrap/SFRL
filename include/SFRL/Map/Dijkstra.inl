namespace rl
{

inline bool Dijkstra::Greater::operator()(const Element& lhs, const Element& rhs) const
{
	if (lhs.second == rhs.second)
	{
		if (lhs.first.y == rhs.first.y)
			return lhs.first.x > rhs.first.x;

		return lhs.first.y > rhs.first.y;
	}

	return lhs.second > rhs.second;
}

inline int Dijkstra::getCost(const sf::Vector2i& position) const
{
	return m_costs[position.x + position.y * m_map->width];
}

inline void Dijkstra::addCost(const sf::Vector2i& position, int cost)
{
	m_costs[position.x + position.y * m_map->width] = cost;
	m_frontier.emplace(position, cost);
}

inline void Dijkstra::clear()
{
	std::fill(m_costs.begin(), m_costs.end(), IntMax);

	while (!m_frontier.empty())
		m_frontier.pop();
}

inline void Dijkstra::compute()
{
	while (!m_frontier.empty())
	{
		const auto [pos, cost] = m_frontier.top();
		m_frontier.pop();

		for (const auto& dir : DIRECTIONS)
		{
			const sf::Vector2i next = pos + dir;

			if (!m_map->isInBounds(next) || !m_map->at(next).passable)
				continue;

			if (getCost(next) > cost + 1)
				addCost(next, cost + 1);
		}
	}
}

inline void Dijkstra::computeVisible()
{
	while (!m_frontier.empty())
	{
		const auto [pos, cost] = m_frontier.top();
		m_frontier.pop();

		for (const auto& dir : DIRECTIONS)
		{
			const sf::Vector2i next = pos + dir;

			if (!m_map->isInBounds(next) || !m_map->at(next).passable || !m_map->at(next).visible)
				continue;

			if (getCost(next) > cost + 1)
				addCost(next, cost + 1);
		}
	}
}

inline void Dijkstra::computeExplored()
{
	while (!m_frontier.empty())
	{
		const auto [pos, cost] = m_frontier.top();
		m_frontier.pop();

		for (const auto& dir : DIRECTIONS)
		{
			const sf::Vector2i next = pos + dir;

			if (!m_map->isInBounds(next) || !m_map->at(next).passable || !m_map->at(next).explored)
				continue;

			if (getCost(next) > cost + 1)
				addCost(next, cost + 1);
		}
	}
}

inline void Dijkstra::computeSafetyMap(const Dijkstra& dijkstra)
{
	m_costs = dijkstra.m_costs;

	int lowestCost = IntMax;
	sf::Vector2i lowestPos;

	for (int y = 0; y < m_map->height; ++y)
		for (int x = 0; x < m_map->width; ++x)
		{
			int& cost = m_costs[x + y * m_map->width];

			if (cost < IntMax)
			{
				cost = static_cast<int>(cost * -1.2f);

				if (cost < lowestCost)
				{
					lowestCost = cost;
					lowestPos.x = x;
					lowestPos.y = y;
				}
			}
		}

	addCost(lowestPos, lowestCost);

	std::vector<bool> visited(m_costs.size(), false);
	visited[lowestPos.x + lowestPos.y * m_map->width] = true;

	while (!m_frontier.empty())
	{
		const auto [pos, cost] = m_frontier.top();
		m_frontier.pop();

		for (const auto& dir : DIRECTIONS)
		{
			const sf::Vector2i next = pos + dir;

			if (!m_map->isInBounds(next) || !m_map->at(next).passable)
				continue;

			const int nextCost = getCost(next);
			const int index = next.x + next.y * m_map->width;

			if (nextCost > cost + 1)
			{
				addCost(next, cost + 1);
				visited[index] = true;
			}

			else if (!visited[index])
			{
				m_frontier.emplace(next, nextCost);
				visited[index] = true;
			}
		}
	}
}

#if 0

inline bool DijkstraOld::Less::operator()(const Element& lhs, const Element& rhs) const
{
	if (lhs.second == rhs.second)
	{
		if (lhs.first.y == rhs.first.y)
			return lhs.first.x < rhs.first.x;

		return lhs.first.y < rhs.first.y;
	}

	return lhs.second < rhs.second;
}

inline int DijkstraOld::getCost(const sf::Vector2i& position) const
{
	return m_costs[position.x + position.y * m_map->width];
}

inline void DijkstraOld::addCost(const sf::Vector2i& position, int cost)
{
	m_costs[position.x + position.y * m_map->width] = cost;
	m_actives.emplace(position, cost);
}

inline void DijkstraOld::clear()
{
	std::fill(m_costs.begin(), m_costs.end(), IntMax);

	m_actives.clear();
}

inline void DijkstraOld::compute()
{
	while (!m_actives.empty())
	{
		const auto [pos, cost] = *m_actives.begin();
		m_actives.erase(m_actives.begin());

		for (const auto& dir : DIRECTIONS)
		{
			const sf::Vector2i next = pos + dir;

			if (!m_map->isInBounds(next) || !m_map->at(next).passable)
				continue;

			if (getCost(next) > cost + 1)
				addCost(next, cost + 1);
		}
	}
}

inline void DijkstraOld::computeSafetyMap(const DijkstraOld& dijkstra)
{
	m_costs = dijkstra.m_costs;

	int lowestCost = IntMax;
	sf::Vector2i lowestPos;

	for (int y = 0; y < m_map->height; ++y)
		for (int x = 0; x < m_map->width; ++x)
		{
			int& cost = m_costs[x + y * m_map->width];

			if (cost < IntMax)
			{
				cost = static_cast<int>(cost * -1.2f);

				if (cost < lowestCost)
				{
					lowestCost = cost;
					lowestPos.x = x;
					lowestPos.y = y;
				}
			}
		}

	addCost(lowestPos, lowestCost);

	std::vector<bool> visited(m_costs.size(), false);
	visited[lowestPos.x + lowestPos.y * m_map->width] = true;

	while (!m_actives.empty())
	{
		const auto [pos, cost] = *m_actives.begin();
		m_actives.erase(m_actives.begin());

		for (const auto& dir : DIRECTIONS)
		{
			const sf::Vector2i next = pos + dir;

			if (!m_map->isInBounds(next) || !m_map->at(next).passable)
				continue;

			const int nextCost = getCost(next);
			const int index = next.x + next.y * m_map->width;

			if (nextCost > cost + 1)
			{
				addCost(next, cost + 1);
				visited[index] = true;
			}

			else if (!visited[index])
			{
				m_actives.emplace(next, nextCost);
				visited[index] = true;
			}
		}
	}
}

#endif

}
