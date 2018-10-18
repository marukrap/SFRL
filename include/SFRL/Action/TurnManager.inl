namespace rl
{

template <typename Actor>
void TurnManager<Actor>::clear()
{
	m_actors.clear();
	m_current = 0;
}

template <typename Actor>
void TurnManager<Actor>::addActor(Actor& actor)
{
	m_actors.emplace_back(&actor);
}

/*
template <typename Actor>
void TurnManager<Actor>::removeActor(Actor& actor)
{
	auto found = std::find_if(m_actors.begin(), m_actors.end(),
		[&] (const auto& a) { return a == &actor; });
	assert(found != m_actors.end());

	m_actors.erase(found);
}
*/

template <typename Actor>
void TurnManager<Actor>::setActors(std::vector<Actor*>&& actors)
{
	m_actors = std::move(actors);
	m_current = 0;
}

template <typename Actor>
bool TurnManager<Actor>::processActions()
{
	if (m_actors.empty())
		return false;

	Actor* actor = nullptr;
	Action* action = nullptr;

	while (!action)
	{
		actor = getCurrentActor();

		if (actor->isWalking())
			return false;

		if (!actor->isDestroyed() && (actor->canTakeTurn() || actor->gainEnergy()))
		{
			if (actor->needsInput())
				return false;

			action = actor->getAction();

			// HACK: vehicle
			if (!action)
				advanceActor();
		}

		else
			advanceActor();
	}

	if (action->perform(*actor))
	{
		actor->spendEnergy();
		actor->finishTurn();

		advanceActor();
	}

	if (action->isDone())
		actor->setAction(nullptr);

	return true;
}

template <typename Actor>
void TurnManager<Actor>::removeWrecks()
{
	Actor* actor = getCurrentActor();

	for (auto it = m_actors.begin(); it != m_actors.end(); )
	{
		if ((*it)->isDestroyed())
		{
			if (*it == actor)
			{
				it = m_actors.erase(it);

				if (it == m_actors.end())
					actor = nullptr;
				else
					actor = *it;
			}

			else
				it = m_actors.erase(it);
		}

		else
			++it;
	}

	m_current = 0;

	for (std::size_t i = 0; i < m_actors.size(); ++i)
	{
		if (m_actors[i] == actor)
		{
			m_current = i;
			break;
		}
	}
}

template <typename Actor>
Actor* TurnManager<Actor>::getCurrentActor() const
{
	return m_actors[m_current];
}

template <typename Actor>
void TurnManager<Actor>::advanceActor()
{
	static const int ticksPerTurn = Energy::ticksPerTurn();

	m_current = (m_current + 1) % m_actors.size();

	if (m_current == 0)
	{
		if (++m_ticks % ticksPerTurn == 0)
			++m_turns;
	}
}

}
