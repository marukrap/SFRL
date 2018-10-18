#pragma once

#include "Map.hpp"
#include "../Rng.hpp"

namespace rl
{

class MapGenerator
{
public:
	using Point = sf::Vector2i;
	using Room = sf::IntRect;

	enum class Passage
	{
		Straight, // path (chebyshev)
		Zigzag,   // corridor (taxicab)
		Winding,  // river
	};

public:
	MapGenerator() = default;
	virtual ~MapGenerator() = default;

	MapGenerator(const MapGenerator&) = delete;
	MapGenerator& operator=(const MapGenerator&) = delete;

	void generate(Map& map, Rng& rng);

protected:
	void fill(Tile tile);
	void fill(int wallProb);

	std::vector<Tile> getTiles() const;
	void setTiles(std::vector<Tile>&& tiles);

	int countTiles(Tile tile) const;
	int countAdjacentTiles(int x, int y, Tile tile) const;

	// cellular automata
	void generation(int r1cutoff); // TODO: better name
	void generation(int r1cutoff, int r2cutoff);

	// remove or connect unconnected regions
	void removeRegions(int removeProb = 100, int minRegionSize = 0);
	void connectRegions(int minRegionSize = 0, Passage passage = Passage::Straight, bool widePassage = true);
	void constructBridges(int minRegionSize = 0);

	// growing tree algorithm
	void growMaze(std::vector<Point>& maze, int x, int y, int windingPercent);
	void removeDeadEnds(std::vector<Point>& maze);

	// lloyd's algorithm (voronoi iteration)
	void relaxation(std::vector<Point>& points);
	void connectPoints(std::vector<Point>& points, Passage passage = Passage::Straight, bool widePassage = true);

	bool canPlaceRoom(const Room& room) const;
	void placeRoom(const Room& room);
	std::vector<Room> fillRandomRooms();

	void carvePath(const Point& from, const Point& to, bool widePassage = true);
	void carveCircle(const Point& center, int radius);
	void carveCorridor(const Point& from, const Point& to);
	void carveWindingRoad(const Point& from, const Point& to, bool widePassage = true, int perturbation = 10);
	void extendLine(Point& from, Point& to);

	void erode(int iterations);
	// void erodeWalls(int r1cutoff);
	void erodeTiles(Tile from, Tile to, int r1cutoff);

	void removeUnusedWalls(); // remove unseen walls

private:
	virtual void onGenerate() = 0;
	virtual void onDecorate() = 0;

protected:
	static constexpr int IntMax = std::numeric_limits<int>::max();

	Map* m_map = nullptr;
	Rng* m_rng = nullptr;

	// TODO: remove
	int m_width = 0;
	int m_height = 0;
	//

	// default tiles
	// unused
	Tile m_floor    = Tile::Floor;
	Tile m_wall     = Tile::Wall;
	Tile m_corridor = Tile::Corridor;
	Tile m_door     = Tile::ClosedDoor;
	// openDoor
	// upStairs
	// downStairs
	Tile m_water    = Tile::Water;
	Tile m_bridge   = Tile::Bridge;
	// chasm?
};

}
