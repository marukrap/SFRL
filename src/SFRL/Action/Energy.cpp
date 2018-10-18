#include "Action/Energy.hpp"

#include <array>
#include <cassert>

namespace
{
	constexpr std::array<int, 11> Gains =
	{
		/*
		 2, // -3 (1/3) //  33% // very slow
		 3, // -2 (1/2) //  50% // quite slow
		 4, // -1 (2/3) //  67% // slow
		 6, //  0 (1x)  // 100% // average
		 9, // +1 (3/2) // 150% // fast
		12, // +2 (2/1) // 200% // quite fast
		18, // +3 (3/1) // 300% // very fast
		*/

		 3, // -5 (1/4) //  25% // extremely slow
		 4, // -4 (1/3) //  33% // very slow
		 6, // -3 (1/2) //  50% // quite slow
		 8, // -2 (2/3) //  67% // slow
		 9, // -1 (3/4) //  75% // below average
		12, //  0 (1x)  // 100% // average
		16, // +1 (4/3) // 133% // above average
		18, // +2 (3/2) // 150% // fast
		24, // +3 (2/1) // 200% // quite fast
		36, // +4 (3/1) // 300% // very fast
		48, // +5 (4/1) // 400% // extremely fast
	};

	constexpr int Min = 0;
	constexpr int Max = Gains.size() - 1;
	constexpr int Normal = Max / 2;

	constexpr int ActionCost = Gains.back();
}

namespace rl
{

int Energy::getSpeed() const
{
	return m_speed;
}

void Energy::setSpeed(int amount)
{
	m_speed = std::clamp(amount, Min - Normal, Max - Normal);
}

void Energy::changeSpeed(int amount)
{
	m_speed = std::clamp(m_speed + amount, Min - Normal, Max - Normal);
}

bool Energy::canTakeTurn() const
{
	// return m_energy >= 0;
	return m_energy >= ActionCost;
}

bool Energy::gainEnergy()
{
	m_energy += Gains[m_speed + Normal];

	return canTakeTurn();
}

void Energy::spendEnergy()
{
	// assert(m_energy >= ActionCost);
	m_energy -= ActionCost;
}

int Energy::ticksPerTurn()
{
	return Gains[Max] / Gains[Normal];
}

bool Energy::needsInput() const
{
	return !m_action;
}

Action* Energy::getAction() const
{
	return m_action.get();
}

void Energy::setAction(Action::Ptr action)
{
	m_action = std::move(action);
}

}
