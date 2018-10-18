#pragma once

#include "Action.hpp"

#include <SFML/Window/Keyboard.hpp>

#include <unordered_map>
#include <functional>

namespace sf
{
	class Event;
}

namespace rl
{

class Player
{
public:
	using Callback = std::function<Action::Ptr(bool, bool, bool)>;

public:
	// TODO: remove enum
	enum class Command
	{
		Unknown = -1,
		MoveW,        // left
		MoveE,        // right
		MoveN,        // up
		MoveS,        // down
		MoveNW,       // left up
		MoveSW,       // left down
		MoveNE,       // right up
		MoveSE,       // right down
		Rest,         // numpad5, (z)
		Interact,     // interact, enter/exit vehicle (e)
		Fire,         // fire (f)
		Reload,       // reload (r)
		SwitchWeapon, // switch weapon (q)
		Throw,        // throw grenade (t)
		NextTarget,   // previous/next target (tab)

		// use item 1 (1)
		// use item 2 (2)
		// use item 3 (3)
		// use item 4 (4)
		// previous weapon ([)
		// next weapon (])
		// fire mode (v)
		// drop current weapon (g)
		// crouch (c)
		// prone (z)
		// sheath weapon (x)

		/*
		c        close (1)
		d        drop (2)
		e        equipment (1)
		E        eat (1)
		f        target (2) and f again to fire (1)
		g        get (1)
		i        inventory (2)
		l        look (3)
		o        open (2) and again to close (1)
		q        quaff (2)
		r        read (2)
		R        rest till interrupted or healed/magicked up (1)
		u        use (1)
		U        special use (1)
		x        exchange (1) , examine (2)
		z        zap (2)
		numpad 0 reload (1)
		12345    quick weapon switch or tactics (1)
		numpad 5 pickup (1)
		.        rest (2)
		,        pickup (2)
		?        help (2)
		/        look (1)
		tab      next target (1)
		enter    action (1)
		<>       up down stairs (2)
		@        character screen (1)
		ctrl x   exit (1)
		*/
	};

public:
	Command getCommand(const sf::Event& event) const;

	// UNDONE: assignKey(s), getAssignedKey(s), bindCommand
	// std::vector<sf::Keyboard::Key> getAssignedKeys(Command command) const;
	// void assignKeys(Command command, std::vector<sf::Keyboard::Key> keys);
	// void bindCommand(Command command, Callback callback);

	Action::Ptr handleEvent(const sf::Event& event);
	Action::Ptr handleRealtimeInput();

protected:
	sf::Keyboard::Key toKey(const std::string& string) const;
	Command toCommand(const std::string& string) const;

	bool isRealtimeCommand(Command command) const;

protected: // private:
	std::unordered_map<sf::Keyboard::Key, Command> m_keyBindings;
	std::unordered_map<Command, Callback> m_commandBindings;
};

}
