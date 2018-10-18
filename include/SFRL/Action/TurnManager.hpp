#pragma once

#include "Action.hpp"

#include <vector>

namespace rl
{

template <typename Actor>
class TurnManager
{
public:
	void clear();
	void addActor(Actor& actor);
	// void removeActor(Actor& actor);
	void setActors(std::vector<Actor*>&& actors);

	bool processActions();
	void removeWrecks();

	// TODO: save/load

private:
	Actor* getCurrentActor() const;
	void advanceActor();

private:
	std::vector<Actor*> m_actors;
	std::size_t m_current = 0;
	std::size_t m_ticks = 0;
	std::size_t m_turns = 0;
};

}

#include "TurnManager.inl"
