#include "Action/Player.hpp"

#include <SFML/Window/Event.hpp>

#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace rl
{

Player::Command Player::getCommand(const sf::Event& event) const
{
	if (event.type == sf::Event::KeyPressed)
	{
		const auto found = m_keyBindings.find(event.key.code);

		if (found != m_keyBindings.end())
			return found->second;
	}

	return Command::Unknown;
}

/*
std::vector<sf::Keyboard::Key> Player::getAssignedKeys(Command command) const
{
	std::vector<sf::Keyboard::Key> keys;

	for (const auto& pair : m_keyBindings)
	{
		if (pair.second == command)
			keys.emplace_back(pair.first);
	}

	return keys;
}

void Player::assignKeys(Command command, std::vector<sf::Keyboard::Key> keys)
{
	for (auto it = m_keyBindings.begin(); it != m_keyBindings.end(); )
	{
		if (it->second == command)
			it = m_keyBindings.erase(it);
		else
			++it;
	}

	for (const auto& key : keys)
		m_keyBindings[key] = command;
}

void Player::bindCommand(Command command, Callback callback)
{
	m_commandBindings[command] = std::move(callback);
}
*/

Action::Ptr Player::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		const auto found = m_keyBindings.find(event.key.code);

		if (found != m_keyBindings.end())
		{
			bool shift = event.key.shift;
			const bool ctrl = event.key.control;
			const bool alt = event.key.alt;

#ifdef _WIN32
			if (!shift && (GetKeyState(VK_NUMLOCK) & 0x0001))
			{
				// HACK: when numlock is toggled if you press numpad keys with shift, it will be converted arrow keys without shift.
				//       this code fixes the problem. but if you press arrow keys (with or without shift), shift key is always considered to be pressed.

				switch (event.key.code)
				{
				case sf::Keyboard::Left:
				case sf::Keyboard::Right:
				case sf::Keyboard::Up:
				case sf::Keyboard::Down:
				case sf::Keyboard::Home:
				case sf::Keyboard::End:
				case sf::Keyboard::PageUp:
				case sf::Keyboard::PageDown:
					shift = true;
					break;
				}
			}
#endif

			return m_commandBindings[found->second](shift, ctrl, alt);
		}

#ifdef _WIN32
		if (GetAsyncKeyState(VK_CLEAR) & 0x8000) // numpad5 with numlock off
		{
			// HACK: when numlock is toggled if you press numpad 5 with shift, it will be converted to VK_CLEAR without shift.
			//       in other words, if numlock is toggled here it means you've pressed numpad 5 with shift.

			const auto found = m_keyBindings.find(sf::Keyboard::Numpad5);

			if (found != m_keyBindings.end())
			{
				const bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) || (GetKeyState(VK_NUMLOCK) & 0x0001);
				const bool ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
				const bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000);

				return m_commandBindings[found->second](shift, ctrl, alt);
			}
		}
#endif
	}

	return nullptr;
}

Action::Ptr Player::handleRealtimeInput()
{
	for (const auto& [key, command] : m_keyBindings)
	{
		if (sf::Keyboard::isKeyPressed(key) && isRealtimeCommand(command))
		{
			const bool shift = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
			const bool ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
			const bool alt = sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt);

			return m_commandBindings[command](shift, ctrl, alt);
		}
	}

	return nullptr;
}

sf::Keyboard::Key Player::toKey(const std::string& string) const
{
	#define TO_SFML_KEY(KEY) if (string == #KEY) return sf::Keyboard::KEY;

	// TO_SFML_KEY(Unknown)
	TO_SFML_KEY(A)
	TO_SFML_KEY(B)
	TO_SFML_KEY(C)
	TO_SFML_KEY(D)
	TO_SFML_KEY(E)
	TO_SFML_KEY(F)
	TO_SFML_KEY(G)
	TO_SFML_KEY(H)
	TO_SFML_KEY(I)
	TO_SFML_KEY(J)
	TO_SFML_KEY(K)
	TO_SFML_KEY(L)
	TO_SFML_KEY(M)
	TO_SFML_KEY(N)
	TO_SFML_KEY(O)
	TO_SFML_KEY(P)
	TO_SFML_KEY(Q)
	TO_SFML_KEY(R)
	TO_SFML_KEY(S)
	TO_SFML_KEY(T)
	TO_SFML_KEY(U)
	TO_SFML_KEY(V)
	TO_SFML_KEY(W)
	TO_SFML_KEY(X)
	TO_SFML_KEY(Y)
	TO_SFML_KEY(Z)
	TO_SFML_KEY(Num0)
	TO_SFML_KEY(Num1)
	TO_SFML_KEY(Num2)
	TO_SFML_KEY(Num3)
	TO_SFML_KEY(Num4)
	TO_SFML_KEY(Num5)
	TO_SFML_KEY(Num6)
	TO_SFML_KEY(Num7)
	TO_SFML_KEY(Num8)
	TO_SFML_KEY(Num9)
	// TO_SFML_KEY(Escape)
	// TO_SFML_KEY(LControl)
	// TO_SFML_KEY(LShift)
	// TO_SFML_KEY(LAlt)
	// TO_SFML_KEY(LSystem)
	// TO_SFML_KEY(RControl)
	// TO_SFML_KEY(RShift)
	// TO_SFML_KEY(RAlt)
	// TO_SFML_KEY(RSystem)
	// TO_SFML_KEY(Menu)
	TO_SFML_KEY(LBracket)
	TO_SFML_KEY(RBracket)
	TO_SFML_KEY(Semicolon)
	TO_SFML_KEY(Comma)
	TO_SFML_KEY(Period)
	TO_SFML_KEY(Quote)
	TO_SFML_KEY(Slash)
	TO_SFML_KEY(Backslash)
	TO_SFML_KEY(Tilde)
	TO_SFML_KEY(Equal)
	TO_SFML_KEY(Hyphen)
	TO_SFML_KEY(Space)
	TO_SFML_KEY(Enter)
	TO_SFML_KEY(Backspace)
	TO_SFML_KEY(Tab)
	TO_SFML_KEY(PageUp)
	TO_SFML_KEY(PageDown)
	TO_SFML_KEY(End)
	TO_SFML_KEY(Home)
	TO_SFML_KEY(Insert)
	TO_SFML_KEY(Delete)
	TO_SFML_KEY(Add)
	TO_SFML_KEY(Subtract)
	TO_SFML_KEY(Multiply)
	TO_SFML_KEY(Divide)
	TO_SFML_KEY(Left)
	TO_SFML_KEY(Right)
	TO_SFML_KEY(Up)
	TO_SFML_KEY(Down)
	TO_SFML_KEY(Numpad0)
	TO_SFML_KEY(Numpad1)
	TO_SFML_KEY(Numpad2)
	TO_SFML_KEY(Numpad3)
	TO_SFML_KEY(Numpad4)
	TO_SFML_KEY(Numpad5)
	TO_SFML_KEY(Numpad6)
	TO_SFML_KEY(Numpad7)
	TO_SFML_KEY(Numpad8)
	TO_SFML_KEY(Numpad9)
	// TO_SFML_KEY(F1)
	// TO_SFML_KEY(F2)
	// TO_SFML_KEY(F3)
	// TO_SFML_KEY(F4)
	// TO_SFML_KEY(F5)
	// TO_SFML_KEY(F6)
	// TO_SFML_KEY(F7)
	// TO_SFML_KEY(F8)
	// TO_SFML_KEY(F9)
	// TO_SFML_KEY(F10)
	// TO_SFML_KEY(F11)
	// TO_SFML_KEY(F12)
	// TO_SFML_KEY(F13)
	// TO_SFML_KEY(F14)
	// TO_SFML_KEY(F15)
	// TO_SFML_KEY(Pause)

	#undef TO_SFML_KEY

	std::cout << "Player::toKey - Unknown key: \"" << string << "\"\n";

	return sf::Keyboard::Unknown;
}

Player::Command Player::toCommand(const std::string& string) const
{
	#define TO_COMMAND(TYPE) if (string == #TYPE) return Command::TYPE;

	TO_COMMAND(MoveW)
	TO_COMMAND(MoveE)
	TO_COMMAND(MoveN)
	TO_COMMAND(MoveS)
	TO_COMMAND(MoveNW)
	TO_COMMAND(MoveSW)
	TO_COMMAND(MoveNE)
	TO_COMMAND(MoveSE)
	TO_COMMAND(Rest)
	TO_COMMAND(Interact)
	TO_COMMAND(Fire)
	TO_COMMAND(Reload)
	TO_COMMAND(SwitchWeapon)
	TO_COMMAND(Throw)
	TO_COMMAND(NextTarget)
	// ...

	#undef TO_COMMAND

	std::cout << "Player::toCommand - Unknown command: \"" << string << "\"\n";

	return Command::Unknown;
}

bool Player::isRealtimeCommand(Command command) const
{
	switch (command)
	{
	case Command::MoveW:
	case Command::MoveE:
	case Command::MoveN:
	case Command::MoveS:
	case Command::MoveNW:
	case Command::MoveSW:
	case Command::MoveNE:
	case Command::MoveSE:
		return true;

	default:
		return false;
	}
}

}
