#include "Direction.hpp"

#include <algorithm>

namespace rl
{

const Direction Direction::None(0, 0);
const Direction Direction::N(0, -1);
const Direction Direction::NE(1, -1);
const Direction Direction::E(1, 0);
const Direction Direction::SE(1, 1);
const Direction Direction::S(0, 1);
const Direction Direction::SW(-1, 1);
const Direction Direction::W(-1, 0);
const Direction Direction::NW(-1, -1);

const std::array<Direction, 8> Direction::All = { N, NE, E, SE, S, SW, W, NW };
const std::array<Direction, 4> Direction::Cardinal = { N, E, S, W };
const std::array<Direction, 4> Direction::Diagonal = { NE, SE, SW, NW };

Direction Direction::operator-() const
{
	return { -x, -y };
}

Direction Direction::left45() const
{
	return { std::clamp(x + y, -1, 1), std::clamp(y - x, -1, 1) };
}

Direction Direction::right45() const
{
	return { std::clamp(x - y, -1, 1), std::clamp(y + x, -1, 1) };
}

Direction Direction::left90() const
{
	return { y, -x };
}

Direction Direction::right90() const
{
	return { -y, x };
}

}
