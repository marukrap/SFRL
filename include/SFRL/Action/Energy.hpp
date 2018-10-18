#pragma once

// credit: http://journal.stuffwithstuff.com/2014/07/15/a-turn-based-game-loop/

#include "Action.hpp"

namespace rl
{

class Energy
{
public:
	int getSpeed() const;
	void setSpeed(int amount);
	void changeSpeed(int amount);

	bool canTakeTurn() const;
	bool gainEnergy();
	void spendEnergy();

	virtual bool needsInput() const;

	Action* getAction() const;
	void setAction(Action::Ptr action);

	template <typename T, typename... Args>
	T* createAction(Args&&... args);

	static int ticksPerTurn();

	// TODO: save/load

private:
	int m_speed = 0;
	int m_energy = 0;
	Action::Ptr m_action = nullptr;
};

template <typename T, typename... Args>
T* Energy::createAction(Args&&... args)
{
	auto action = std::make_unique<T>(std::forward<Args>(args)...);

	T* result = action.get();
	setAction(std::move(action));

	return result;
}

}
