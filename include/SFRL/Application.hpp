#pragma once

#include "ResourceManager.hpp"
#include "StateStack.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace rl
{

class Application
{
public:
	void run();

protected:
	Application();

	void initializeFpsText(const sf::Font& font, int fontSize);
	void initializeDebugGrid(const sf::Vector2i& gridSize);
	void initializeFilter(/* FilterType filter */);

	void processInput();
	void update(sf::Time dt);
	void render();

	void centerWindow();
	void takeScreenshot();
	void countFrame(sf::Time dt);
	void drawGrid();

protected:
	ResourceManager m_resources;
	StateStack m_stateStack;
	sf::RenderWindow m_window;
	sf::Color m_clearColor = sf::Color::Black;

	sf::Text m_fpsText;
	sf::Sprite m_gridSprite;
	sf::Sprite m_filterSprite;
	sf::Vector2i m_gridSize;

	bool m_displayFps = false;
	bool m_displayGrid = false;
	bool m_displayFilter = false;
};

}
