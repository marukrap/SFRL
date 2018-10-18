#include "Map/MapGenerator.hpp"
#include "Direction.hpp"
#include "Utility.hpp"

#include <queue>
#include <list>
#include <cassert>

namespace rl
{

void MapGenerator::generate(Map& map, Rng& rng)
{
	m_map = &map;
	m_rng = &rng;
	m_width = map.width;
	m_height = map.height;

	// TODO: save rng seed for tile map
	// m_rng->printSeed();

	onGenerate();
	// onDecorate();

	// initialize map flags
	for (int y = 0; y < m_height; ++y)
		for (int x = 0; x < m_width; ++x)
		{
			switch (m_map->getTile(x, y))
			{
			case Tile::Floor:
			case Tile::Corridor:
			case Tile::OpenDoor:
			case Tile::UpStairs:
			case Tile::DownStairs:
			case Tile::Bridge:
				// m_map->resetFlags(x, y, Map::Passable | Map::Transparent);
				m_map->at(x, y).passable = true;
				m_map->at(x, y).transparent = true;
				break;

			case Tile::Water:
				// m_map->resetFlags(x, y, Map::Transparent);
				m_map->at(x, y).passable = false;
				m_map->at(x, y).transparent = true;
				break;

			default: // unused, wall, closed door
				// m_map->resetFlags(x, y, Map::None);
				m_map->at(x, y).passable = false;
				m_map->at(x, y).transparent = false;
				break;
			}
		}

	onDecorate();
}

void MapGenerator::fill(Tile tile)
{
	std::fill(m_map->m_tiles.begin(), m_map->m_tiles.end(), tile);
}

void MapGenerator::fill(int wallProb)
{
	for (int y = 0; y < m_height; ++y)
		for (int x = 0; x < m_width; ++x)
		{
			if (x == 0 || y == 0 || x == m_width - 1 || y == m_height - 1)
				m_map->setTile(x, y, m_wall);
			else if (m_rng->getInt(100) < wallProb)
				m_map->setTile(x, y, m_wall);
			else
				m_map->setTile(x, y, m_floor);
		}
}

std::vector<Tile> MapGenerator::getTiles() const
{
	return m_map->m_tiles;
}

void MapGenerator::setTiles(std::vector<Tile>&& tiles)
{
	m_map->m_tiles = std::move(tiles);
}

int MapGenerator::countTiles(Tile tile) const
{
	return std::count_if(m_map->m_tiles.begin(), m_map->m_tiles.end(), [&] (const auto& t) { return t == tile; });
}

int MapGenerator::countAdjacentTiles(int x, int y, Tile tile) const
{
	const int left   = std::max(0, x - 1);
	const int top    = std::max(0, y - 1);
	const int right  = std::min(x + 1, m_width - 1);
	const int bottom = std::min(y + 1, m_height - 1);

	int count = 0;

	for (int j = top; j <= bottom; ++j)
		for (int i = left; i <= right; ++i)
		{
			if (m_map->getTile(i, j) == tile)
				++count;
		}

	return count;
}

void MapGenerator::generation(int r1cutoff)
{
	std::vector<Tile> tiles(m_width * m_height, m_wall);

	for (int y = 1; y < m_height - 1; ++y)
		for (int x = 1; x < m_width - 1; ++x)
		{
			const int r1 = countAdjacentTiles(x, y, m_wall);

			if (r1 >= r1cutoff)
				tiles[x + y * m_width] = m_wall;
			else
				tiles[x + y * m_width] = m_floor;
		}

	m_map->m_tiles = std::move(tiles);
}

void MapGenerator::generation(int r1cutoff, int r2cutoff)
{
	std::vector<Tile> tiles(m_width * m_height, m_wall);

	for (int y = 1; y < m_height - 1; ++y)
		for (int x = 1; x < m_width - 1; ++x)
		{
			int r1 = 0;
			int r2 = 0;

			for (int dy = -2; dy <= 2; ++dy)
				for (int dx = -2; dx <= 2; ++dx)
				{
					const int ax = std::abs(dx);
					const int ay = std::abs(dy);

					if (ax == 2 && ay == 2)
						continue;

					if (m_map->isInBounds(x + dx, y + dy) &&
						m_map->getTile(x + dx, y + dy) == m_wall)
					{
						if (ax <= 1 && ay <= 1)
							++r1;

						++r2;
					}
				}

			if (r1 >= r1cutoff || r2 <= r2cutoff)
				tiles[x + y * m_width] = m_wall;
			else
				tiles[x + y * m_width] = m_floor;
		}

	m_map->m_tiles = std::move(tiles);
}

void MapGenerator::removeRegions(int removeProb, int minRegionSize)
{
	int currentRegion = -1;
	std::vector<int> regions(m_width * m_height, currentRegion);
	std::vector<int> regionsSizes;

	// non-recursive flood fill
	for (int y = 1; y < m_height - 1; ++y)
		for (int x = 1; x < m_width - 1; ++x)
		{
			if (m_map->getTile(x, y) == m_wall || regions[x + y * m_width] >= 0)
				continue;

			currentRegion += 1;
			regionsSizes.emplace_back(0);

			std::queue<Point> queue;
			queue.emplace(x, y);

			while (!queue.empty())
			{
				const Point pos = queue.front();
				queue.pop();

				if (m_map->getTile(pos) == m_wall || regions[pos.x + pos.y * m_width] >= 0)
					continue;

				regions[pos.x + pos.y * m_width] = currentRegion;
				regionsSizes[currentRegion] += 1;

				for (const auto& dir : Direction::Cardinal)
				{
					if (m_map->isInBounds(pos + dir))
						queue.emplace(pos + dir);
				}
			}
		}

	// find the biggest region
	int biggestRegion = 0;
	std::vector<bool> regionsForRemoval(currentRegion + 1, false);

	for (int i = 0; i <= currentRegion; ++i)
	{
		if (regionsSizes[i] > regionsSizes[biggestRegion])
			biggestRegion = i;

		if (m_rng->getInt(100) < removeProb || regionsSizes[i] < minRegionSize)
			regionsForRemoval[i] = true;
	}

	// do not remove the biggest region
	regionsForRemoval[biggestRegion] = false;

	// remove marked regions
	for (int y = 1; y < m_height - 1; ++y)
		for (int x = 1; x < m_width - 1; ++x)
		{
			if (m_map->getTile(x, y) == m_wall)
				continue;

			const int i = regions[x + y * m_width];

			if (regionsForRemoval[i])
				m_map->setTile(x, y, m_wall);
		}
}

void MapGenerator::connectRegions(int minRegionSize, Passage passage, bool widePassage)
{
	int currentRegion = -1;
	std::vector<int> regions(m_width * m_height, currentRegion);
	std::vector<int> regionsSizes;
	std::vector<std::vector<Point>> connectors;

	// non-recursive flood fill
	for (int y = 1; y < m_height - 1; ++y)
		for (int x = 1; x < m_width - 1; ++x)
		{
			if (m_map->getTile(x, y) == m_wall || regions[x + y * m_width] >= 0)
				continue;

			currentRegion += 1;
			regionsSizes.emplace_back(0);
			connectors.emplace_back();

			std::queue<Point> queue;
			queue.emplace(x, y);

			while (!queue.empty())
			{
				const Point pos = queue.front();
				queue.pop();

				if (m_map->getTile(pos) == m_wall || regions[pos.x + pos.y * m_width] >= 0)
					continue;

				regions[pos.x + pos.y * m_width] = currentRegion;
				regionsSizes[currentRegion] += 1;

				bool isConnector = false;

				for (const auto& dir : Direction::Cardinal)
				{
					if (m_map->isInBounds(pos + dir))
					{
						queue.emplace(pos + dir);

						if (m_map->getTile(pos + dir) == m_wall)
							isConnector = true;
					}
				}

				if (isConnector)
					connectors[currentRegion].emplace_back(pos);
			}
		}

	// find the biggest region
	int biggestRegion = 0;
	std::vector<bool> regionsForRemoval(currentRegion + 1, false);

	for (int i = 0; i <= currentRegion; ++i)
	{
		if (regionsSizes[i] > regionsSizes[biggestRegion])
			biggestRegion = i;

		if (regionsSizes[i] < minRegionSize)
			regionsForRemoval[i] = true;
	}

	// do not remove the biggest region
	regionsForRemoval[biggestRegion] = false;

	// remove marked regions
	for (int y = 1; y < m_height - 1; ++y)
		for (int x = 1; x < m_width - 1; ++x)
		{
			if (m_map->getTile(x, y) == m_wall)
				continue;

			const int i = regions[x + y * m_width];

			if (regionsForRemoval[i])
				m_map->setTile(x, y, m_wall);
		}

	std::vector<int> connected;
	std::list<int> unconnected;

	for (int i = 0; i <= currentRegion; ++i)
	{
		if (regionsForRemoval[i])
			continue;

		if (i == biggestRegion)
			connected.emplace_back(i);
		else
			unconnected.emplace_back(i);
	}

	while (!unconnected.empty())
	{
		std::vector<std::pair<Point, Point>> bestConnectors; // from, to
		int bestDistance = IntMax;

		for (const int i : connected)
			for (const auto& from : connectors[i])
				for (const int j : unconnected)
					for (const auto& to : connectors[j])
					{
						const Point delta = to - from;
						const int distance = std::max(std::abs(delta.x), std::abs(delta.y));

						/*
						switch (passage)
						{
						case Passage::Straight:
							distance = lengthSquared(delta);
							break;

						case Passage::Zigzag:
						case Passage::Winding:
							if (allowDiagonalSteps)
								distance = std::max(std::abs(delta.x), std::abs(delta.y));
							else
								distance = std::abs(delta.x) + std::abs(delta.y);
							break;
						}
						*/

						if (distance < bestDistance)
						{
							bestConnectors.clear();
							bestConnectors.emplace_back(from, to);
							bestDistance = distance;
						}

						else if (distance == bestDistance)
							bestConnectors.emplace_back(from, to);
					}

		assert(!bestConnectors.empty());

		const auto [bestFrom, bestTo] = m_rng->getOne(bestConnectors);
		const int bestToIndex = regions[bestTo.x + bestTo.y * m_width];

		switch (passage)
		{
		case Passage::Straight: carvePath(bestFrom, bestTo, widePassage); break;
		case Passage::Zigzag:   carveCorridor(bestFrom, bestTo); break;
		case Passage::Winding:  carveWindingRoad(bestFrom, bestTo, widePassage); break;
		}

		connected.emplace_back(bestToIndex);
		unconnected.remove(bestToIndex);
	}
}

void MapGenerator::constructBridges(int minRegionSize)
{
	// UNDONE: reduce duplicated code from connectRegions()

	struct Connector
	{
		Connector(const Point& pos, const Direction& dir)
			: pos(pos)
			, dir(dir)
		{
		}

		Point pos;
		Direction dir;
		int length = 0;
	};

	const auto isPassable = [this] (const Point& pos)
	{
		return m_map->getTile(pos) == m_floor || m_map->getTile(pos) == m_corridor;
	};

	int currentRegion = -1;
	std::vector<int> regions(m_width * m_height, currentRegion);
	std::vector<int> regionsSizes;
	std::vector<std::vector<Connector>> connectors;

	// non-recursive flood fill
	for (int y = 1; y < m_height - 1; ++y)
		for (int x = 1; x < m_width - 1; ++x)
		{
			if (!isPassable({ x, y }) || regions[x + y * m_width] >= 0)
				continue;

			currentRegion += 1;
			regionsSizes.emplace_back(0);
			connectors.emplace_back();

			std::queue<Point> queue;
			queue.emplace(x, y);

			while (!queue.empty())
			{
				const Point pos = queue.front();
				queue.pop();

				if (!isPassable(pos) || regions[pos.x + pos.y * m_width] >= 0)
					continue;

				regions[pos.x + pos.y * m_width] = currentRegion;
				regionsSizes[currentRegion] += 1;

				for (const auto& dir : Direction::Cardinal)
				{
					if (m_map->isInBounds(pos + dir))
					{
						queue.emplace(pos + dir);

						if (!isPassable(pos + dir))
							connectors[currentRegion].emplace_back(pos, dir);
					}
				}
			}
		}

	// find the biggest region
	int biggestRegion = 0;
	std::vector<bool> regionsForRemoval(currentRegion + 1, false);

	for (int i = 0; i <= currentRegion; ++i)
	{
		if (regionsSizes[i] > regionsSizes[biggestRegion])
			biggestRegion = i;

		if (regionsSizes[i] < minRegionSize)
			regionsForRemoval[i] = true;
	}

	// do not remove the biggest region
	regionsForRemoval[biggestRegion] = false;

	// remove marked regions
	for (int y = 1; y < m_height - 1; ++y)
		for (int x = 1; x < m_width - 1; ++x)
		{
			if (!isPassable({ x, y }))
				continue;

			const int i = regions[x + y * m_width];

			if (regionsForRemoval[i])
				m_map->setTile(x, y, m_water); // UNDONE: bug?
		}

	std::vector<int> connected;
	std::list<int> unconnected;

	for (int i = 0; i <= currentRegion; ++i)
	{
		if (regionsForRemoval[i])
			continue;

		if (i == biggestRegion)
			connected.emplace_back(i);
		else
			unconnected.emplace_back(i);
	}

	while (!unconnected.empty())
	{
		std::vector<Connector*> bestConnectors;
		int bestDistance = IntMax;

		for (int from : connected)
			for (Connector& connector : connectors[from])
			{
				if (connector.length < 0)
					continue;

				Point pos = connector.pos;
				connector.length = 0;

				while (true)
				{
					pos += connector.dir;
					connector.length += 1;

					if (!m_map->isInBounds(pos))
					{
						connector.length = -1; // disable the connector
						break;
					}

					const int to = regions[pos.x + pos.y * m_width];

					if (to < 0)
						continue;

					const auto found = std::find(unconnected.begin(), unconnected.end(), to);

					if (found != unconnected.end())
					{
						if (connector.length < bestDistance)
						{
							bestConnectors.clear();
							bestConnectors.emplace_back(&connector);
							bestDistance = connector.length;
						}

						else if (connector.length == bestDistance)
							bestConnectors.emplace_back(&connector);
					}

					else
						connector.length = -1;

					break;
				}
			}

		if (bestConnectors.empty())
		{
			// NOTE: this function only construct straight bridges
			//       in other words, diagonally separated areas may not be connected

			for (int i : unconnected)
				regionsForRemoval[i] = true;

			for (int y = 1; y < m_height - 1; ++y)
				for (int x = 1; x < m_width - 1; ++x)
				{
					if (!isPassable({ x, y }))
						continue;

					const int i = regions[x + y * m_width];

					if (regionsForRemoval[i])
						m_map->setTile(x, y, m_water);
				}

			break;
		}

		Connector* bestFrom = m_rng->getOne(bestConnectors);
		Point bestToPos = bestFrom->pos + bestFrom->dir * bestFrom->length;
		int bestToIndex = regions[bestToPos.x + bestToPos.y * m_width];

		for (int i = 1; i < bestFrom->length; ++i)
		{
			const Point pos = bestFrom->pos + bestFrom->dir * i;

			if (m_map->getTile(pos) == m_water)
				m_map->setTile(pos, m_bridge);
			else
				m_map->setTile(pos, m_corridor);

			// add new connectors from the constructed bridges/corridors
			for (const auto& dir : Direction::Cardinal)
			{
				if (!isPassable(pos + dir))
					connectors[bestToIndex].emplace_back(pos, dir);
			}
		}

		connected.emplace_back(bestToIndex);
		unconnected.remove(bestToIndex);
	}
}

void MapGenerator::growMaze(std::vector<Point>& maze, int x, int y, int windingPercent)
{
	if (countAdjacentTiles(x, y, m_wall) < 9)
		return;

	const auto canCarve = [this] (const Point& pos, const Direction& dir)
	{
		if (!m_map->isInBounds(pos + dir * 3))
			return false;

		Point left = pos + dir + dir.left45();
		Point right = pos + dir + dir.right45();

		if (m_map->getTile(left) != m_wall || m_map->getTile(right) != m_wall)
			return false;

		left += dir;
		right += dir;

		if (m_map->getTile(left) != m_wall || m_map->getTile(right) != m_wall)
		{
			// m_map->setTile(pos + dir, m_corridor);
			return false;
		}

		return m_map->getTile(pos + dir * 2) == m_wall;
	};

	std::vector<Point> cells;
	Direction lastDir = Direction::None;

	m_map->setTile(x, y, m_corridor);
	maze.emplace_back(x, y);
	cells.emplace_back(x, y);

	while (!cells.empty())
	{
		const Point cell = cells.back();
		std::vector<Direction> unmadeCells;

		for (const auto& dir : Direction::Cardinal)
		{
			if (canCarve(cell, dir))
				unmadeCells.emplace_back(dir);
		}

		if (!unmadeCells.empty())
		{
			const auto found = std::find(unmadeCells.begin(), unmadeCells.end(), lastDir);

			if (found == unmadeCells.end() || m_rng->getInt(100) < windingPercent)
				lastDir = m_rng->getOne(unmadeCells);

			m_map->setTile(cell + lastDir, m_corridor);
			m_map->setTile(cell + lastDir * 2, m_corridor);

			maze.emplace_back(cell + lastDir);
			maze.emplace_back(cell + lastDir * 2);

			cells.emplace_back(cell + lastDir * 2);
		}

		else
		{
			cells.pop_back();
			lastDir = Direction::None;
		}
	}
}

void MapGenerator::removeDeadEnds(std::vector<Point>& maze)
{
	while (!maze.empty())
	{
		const Point pos = maze.back();
		maze.pop_back();

		if (m_map->getTile(pos) == m_wall)
			continue;

		int exits = 0;

		for (const auto& dir : Direction::Cardinal)
		{
			if (m_map->getTile(pos + dir) != m_wall)
				++exits;
		}

		if (exits <= 1)
		{
			m_map->setTile(pos, m_wall);

			for (const auto& dir : Direction::Cardinal)
				maze.emplace_back(pos + dir);
		}
	}
}

void MapGenerator::relaxation(std::vector<Point>& points)
{
	std::vector<std::pair<Point, int>> regions;

	for (const auto& pos : points)
		regions.emplace_back(pos, 1);

	for (int y = 0; y < m_height; ++y)
		for (int x = 0; x < m_width; ++x)
		{
			const Point pos(x, y);
			int nearest = -1;
			int nearestDistance = IntMax;

			for (std::size_t i = 0; i < points.size(); ++i)
			{
				const int distance = lengthSquared(points[i] - pos);

				if (distance < nearestDistance)
				{
					nearest = i;
					nearestDistance = distance;
				}
			}

			regions[nearest].first += pos;
			regions[nearest].second += 1;
		}

	for (std::size_t i = 0; i < points.size(); ++i)
		points[i] = regions[i].first / regions[i].second;
}

void MapGenerator::connectPoints(std::vector<Point>& points, Passage passage, bool widePassage)
{
	std::vector<Point> connected;

	connected.emplace_back(points.back());
	points.pop_back();

	while (!points.empty())
	{
		Point bestFrom;
		int bestToIndex = -1;
		int bestDistance = IntMax;

		for (const auto& from : connected)
			for (std::size_t i = 0; i < points.size(); ++i)
			{
				const int distance = lengthSquared(points[i] - from);

				if (distance < bestDistance)
				{
					bestFrom = from;
					bestToIndex = i;
					bestDistance = distance;
				}
			}

		const Point bestTo = points[bestToIndex];

		switch (passage)
		{
		case Passage::Straight: carvePath(bestFrom, bestTo, widePassage); break;
		case Passage::Zigzag:   carveCorridor(bestFrom, bestTo); break;
		case Passage::Winding:  carveWindingRoad(bestFrom, bestTo, widePassage); break;
		}

		connected.emplace_back(bestTo);
		points.erase(points.begin() + bestToIndex);
	}

	connected.swap(points);
}

bool MapGenerator::canPlaceRoom(const Room& room) const
{
	const int left   = room.left - 1;
	const int top    = room.top - 1;
	const int right  = room.left + room.width + 1;
	const int bottom = room.top + room.height + 1;

	for (int y = top; y < bottom; ++y)
		for (int x = left; x < right; ++x)
		{
			if (m_map->getTile(x, y) != m_wall)
				return false;
		}

	return true;
}

void MapGenerator::placeRoom(const Room& room)
{
	const int left   = room.left;
	const int top    = room.top;
	const int right  = room.left + room.width;
	const int bottom = room.top + room.height;

	for (int y = top; y < bottom; ++y)
		for (int x = left; x < right; ++x)
			m_map->setTile(x, y, m_floor);
}

std::vector<MapGenerator::Room> MapGenerator::fillRandomRooms()
{
	// TODO: non-rectangular rooms

	std::vector<Point> points;

	for (int y = 1; y < m_height - 1; ++y)
		for (int x = 1; x < m_width - 1; ++x)
		{
			if (m_map->getTile(x, y) == m_wall)
				points.emplace_back(x, y);
		}

	m_rng->shuffle(points);

	std::vector<Room> rooms;

	while (!points.empty())
	{
		const Point pos = points.back();
		points.pop_back();

		Room room;
		room.left   = pos.x;
		room.top    = pos.y;
		room.width  = m_rng->rollDice(3, 3);
		room.height = m_rng->rollDice(3, 3);

		if (room.left + room.width >= m_width || room.top + room.height >= m_height)
			continue;

		if (canPlaceRoom(room))
		{
			/*
			// TODO: fix this later (incorrect?)
			bool valid = true;

			// HACK: avoid double walls
			for (const auto& other : rooms)
			{
				if ((room.left + room.width + 2 == other.left) || (room.top + room.height + 2 == other.top) ||
					(other.left + other.width + 2 == room.left) || (other.top + other.height + 2 == room.top))
				{
					valid = false;
					break;
				}
			}

			if (valid)
			*/
			{
				placeRoom(room);
				rooms.emplace_back(std::move(room));
			}
		}
	}

	return rooms;
}

void MapGenerator::carvePath(const Point& from, const Point& to, bool widePassage)
{
	const std::vector<Point> line = plotLine(from, to, !widePassage);

	for (const auto& pos : line)
	{
		m_map->setTile(pos, m_corridor);

		if (widePassage)
		{
			if (pos.x + 1 < m_width - 1)
				m_map->setTile(pos.x + 1, pos.y, m_corridor);
			if (pos.y + 1 < m_height - 1)
				m_map->setTile(pos.x, pos.y + 1, m_corridor);
		}
	}
}

void MapGenerator::carveCircle(const Point& center, int radius)
{
	assert(radius >= 0);

	const int left   = std::max(1, center.x - radius);
	const int top    = std::max(1, center.y - radius);
	const int right  = std::min(center.x + radius, m_width - 2);
	const int bottom = std::min(center.y + radius, m_height - 2);

	for (int y = top; y <= bottom; ++y)
		for (int x = left; x <= right; ++x)
		{
			// NOTE: < or <=
			if (lengthSquared(Point(x, y) - center) <= radius * radius)
				m_map->setTile(x, y, m_corridor);
		}
}

void MapGenerator::carveCorridor(const Point& from, const Point& to)
{
	const Point delta = to - from;
	Point primaryIncrement(sign(delta.x), 0);
	Point secondaryIncrement(0, sign(delta.y));
	int primary = std::abs(delta.x);
	int secondary = std::abs(delta.y);

	const Tile t1 = m_map->getTile(from + primaryIncrement);
	const Tile t2 = m_map->getTile(from + secondaryIncrement);

	if ((t1 != m_wall && t2 == m_wall) ||
		(t1 == m_wall && t2 == m_wall && m_rng->getBool()))
	{
		std::swap(primary, secondary);
		std::swap(primaryIncrement, secondaryIncrement);
	}

	/*
	if (m_rng->getBool())
	{
		std::swap(primary, secondary);
		std::swap(primaryIncrement, secondaryIncrement);
	}
	*/

	std::vector<Point> line;
	Point current = from;
	int windingPoint = -1;

	if (primary > 3)
		windingPoint = m_rng->getInt(2, primary - 2);

	while (true)
	{
		line.emplace_back(current);

		if (primary > 0 && (primary != windingPoint || secondary == 0))
		{
			current += primaryIncrement;
			--primary;
		}

		else if (secondary > 0)
		{
			current += secondaryIncrement;
			--secondary;
		}

		else
		{
			assert(current == to);
			break;
		}
	}

	for (std::size_t i = 1; i < line.size() - 1; ++i)
	{
		if (m_map->getTile(line[i]) == m_wall)
			m_map->setTile(line[i], m_corridor);
	}

	// TODO: place doors
}

void MapGenerator::carveWindingRoad(const Point& from, const Point& to, bool widePassage, int perturbation)
{
	// credit: http://www.roguebasin.com/index.php?title=Winding_ways

	// the square of the cosine of the angle between vectors p0p1 and p1p2,
	// with the sign of the cosine, in permil (1.0 = 1000).
	const auto signcos2 = [] (const Point& p0, const Point& p1, const Point& p2)
	{
		const int sqlen01 = lengthSquared(p1 - p0);
		const int sqlen12 = lengthSquared(p2 - p1);
		const int prod = (p1.x - p0.x) * (p2.x - p1.x) + (p1.y - p0.y) * (p2.y - p1.y);
		const int val = 1000 * (prod * prod / sqlen01) / sqlen12;

		return prod < 0 ? -val : val;
	};

	std::vector<Point> line = plotLine(from, to);

	if (line.size() >= 5)
	{
		std::size_t j = 0;

		for (std::size_t i = 0; i < line.size(); )
		{
			line[j++] = line[i];

			if (i < line.size() - 5 || i >= line.size() - 1)
				i += m_rng->getInt(2, 3);
			else if (i == line.size() - 5)
				i += 2;
			else
				i = line.size() - 1;
		}

		line.resize(j);

		if (line.size() >= 3)
		{
			const int mind2 = 2 * 2; // mindist = 2
			const int maxd2 = 5 * 5; // maxdist = 5
			const int mincos2 = 500; // cos^2 in 1/1000, for angles < 45 degrees

			for (std::size_t i = 0; i < j * perturbation; ++i)
			{
				const std::size_t ri = 1 + m_rng->getInt(j - 2);
				const Direction rdir = Direction::All[m_rng->getInt(Direction::All.size())];
				const Point rpos = line[ri] + rdir;

				const int lod2 = lengthSquared(rpos - line[ri - 1]);
				const int hid2 = lengthSquared(rpos - line[ri + 1]);

				if (!m_map->isInBounds(rpos) ||
					lod2 < mind2 || lod2 > maxd2 ||
					hid2 < mind2 || hid2 > maxd2)
					continue;

				if (signcos2(line[ri - 1], rpos, line[ri + 1]) < mincos2)
					continue;

				if (ri > 1 && signcos2(line[ri - 2], line[ri - 1], rpos) < mincos2)
					continue;

				if (ri < line.size() - 2 && signcos2(rpos, line[ri + 1], line[ri + 2]) < mincos2)
					continue;

				line[ri] = rpos;
			}
		}
	}

	const int radius = widePassage ? 1 : 0; // the breadth of the river

	for (std::size_t i = 0; i < line.size() - 1; ++i)
	{
		const std::vector<Point> subline = plotLine(line[i], line[i + 1], !widePassage);

		for (const auto& pos : subline)
		{
			const int left   = std::max(0, pos.x - radius);
			const int top    = std::max(0, pos.y - radius);
			const int right  = std::min(pos.x + radius, m_width - 1);
			const int bottom = std::min(pos.y + radius, m_height - 1);

			for (int y = top; y <= bottom; ++y)
				for (int x = left; x <= right; ++x)
				{
					if (lengthSquared(Point(x, y) - pos) <= radius * radius)
						m_map->setTile(x, y, m_corridor);
				}
		}
	}
}

void MapGenerator::extendLine(Point& from, Point& to)
{
	// UNDONE:

	assert(from != to);

	const Point delta = to - from;
	Point primaryIncrement(sign(delta.x), 0);
	Point secondaryIncrement(0, sign(delta.y));
	int primary = std::abs(delta.x);
	int secondary = std::abs(delta.y);

	if (secondary > primary)
	{
		std::swap(primary, secondary);
		std::swap(primaryIncrement, secondaryIncrement);
	}

	for (int error = 0; from.x > 0 && from.x < m_width - 1 && from.y > 0 && from.y < m_height - 1; )
	{
		from -= primaryIncrement;
		error += secondary;

		if (error * 2 >= primary)
		{
			from -= secondaryIncrement;
			error -= primary;
		}
	}

	for (int error = 0; to.x > 0 && to.x < m_width - 1 && to.y > 0 && to.y < m_height - 1; )
	{
		to += primaryIncrement;
		error += secondary;

		if (error * 2 >= primary)
		{
			to += secondaryIncrement;
			error -= primary;
		}
	}
}

void MapGenerator::erode(int iterations)
{
	for (int i = 0; i < iterations; ++i)
	{
		Point pos;
		pos.x = m_rng->getInt(1, m_width - 2);
		pos.y = m_rng->getInt(1, m_height - 2);

		if (m_map->getTile(pos) != m_wall)
			continue;

		int floors = 0;

		for (const auto& dir : Direction::All)
		{
			if (m_map->getTile(pos + dir) == m_floor)
				++floors;
		}

		if (floors >= 2 && m_rng->getInt(9 - floors) == 0)
			m_map->setTile(pos, m_floor);
	}
}

/*
void MapGenerator::erodeWalls(int r1cutoff)
{
	std::vector<bool> wallsForRemoval(m_width * m_height, false);

	for (int y = 1; y < m_height - 1; ++y)
		for (int x = 1; x < m_width - 1; ++x)
		{
			if (m_map->getTile(x, y) != m_wall)
				continue;

			if (countAdjacentTiles(x, y, m_floor) >= r1cutoff)
				wallsForRemoval[x + y * m_width] = true;
		}

	for (int y = 1; y < m_height -1; ++y)
		for (int x = 1; x < m_width - 1; ++x)
		{
			if (wallsForRemoval[x + y * m_width])
				m_map->setTile(x, y, m_floor);
		}
}
*/

void MapGenerator::erodeTiles(Tile from, Tile to, int r1cutoff)
{
	std::vector<bool> tilesForRemoval(m_width * m_height, false);

	for (int y = 1; y < m_height - 1; ++y)
		for (int x = 1; x < m_width - 1; ++x)
		{
			if (m_map->getTile(x, y) != from)
				continue;

			if (countAdjacentTiles(x, y, to) >= r1cutoff)
				tilesForRemoval[x + y * m_width] = true;
		}

	for (int y = 1; y < m_height - 1; ++y)
		for (int x = 1; x < m_width - 1; ++x)
		{
			if (tilesForRemoval[x + y * m_width])
				m_map->setTile(x, y, to);
		}
}

void MapGenerator::removeUnusedWalls()
{
	for (int y = 0; y < m_height; ++y)
		for (int x = 0; x < m_width; ++x)
		{
			const Point pos(x, y);

			if (m_map->getTile(pos) != m_wall)
				continue;

			bool removeWall = true;

			for (const auto& dir : Direction::All)
			{
				if (!m_map->isInBounds(pos + dir))
					continue;

				if (m_map->getTile(pos + dir) != m_wall &&
					m_map->getTile(pos + dir) != Tile::Unused)
				{
					removeWall = false;
					break;
				}
			}

			if (removeWall)
				m_map->setTile(x, y, Tile::Unused);
		}
}

}
