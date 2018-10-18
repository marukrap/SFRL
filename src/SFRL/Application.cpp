#include "Application.hpp"
#include "Action/Action.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>

#include <sstream>
#include <filesystem>
#include <iomanip>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace fs = std::filesystem;

namespace rl
{

void Application::run()
{
	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate;

	while (m_window.isOpen())
	{
		const sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;

		if (timeSinceLastUpdate >= timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;

			processInput();
			update(timePerFrame);

			if (m_stateStack.isEmpty())
				m_window.close();
		}

		if (m_displayFps)
			countFrame(dt);

		render();
	}
}

Application::Application()
{
	Action::setStateStack(m_stateStack);
}

void Application::initializeFpsText(const sf::Font& font, int fontSize)
{
	m_fpsText.setFont(font);
	m_fpsText.setCharacterSize(fontSize);
	m_fpsText.setOutlineThickness(1.f);
	m_fpsText.setPosition(5.f, 5.f);

	const sf::FloatRect bounds = sf::Text('A', font, fontSize).getLocalBounds();
	m_fpsText.setOrigin(bounds.left, bounds.top);
}

void Application::initializeDebugGrid(const sf::Vector2i& gridSize)
{
	static const sf::Texture texture = [gridSize] ()
	{
		sf::Image image;
		image.create(gridSize.x, gridSize.y);

		for (int y = 0; y < gridSize.y; ++y)
			for (int x = 0; x < gridSize.x; ++x)
			{
				if ((x == 0 || y == 0) && (x % 2 == 0 && y % 2 == 0))
					image.setPixel(x, y, { 255, 255, 255, 51 });
				else
					image.setPixel(x, y, { 0, 0, 0, 0 });
			}

		sf::Texture texture;
		texture.loadFromImage(image);
		texture.setRepeated(true);

		return texture;
	}();

	const int width = m_window.getSize().x;
	const int height = m_window.getSize().y;

	m_gridSprite.setTexture(texture);
	m_gridSprite.setTextureRect({ 0, 0, width, height });
	m_gridSize = gridSize;
}

void Application::initializeFilter()
{
	// TODO: use sf::Shader, more filter types

	static const sf::Texture texture = [] ()
	{
		sf::Image image;
		image.create(1, 2);

		for (unsigned int y = 0; y < image.getSize().y; ++y)
			for (unsigned int x = 0; x < image.getSize().x; ++x)
			{
				if (y % 2 == 0)
					image.setPixel(x, y, { 0, 0, 0, 0 });
				else
					image.setPixel(x, y, { 0, 0, 0, 51 });
			}

		sf::Texture texture;
		texture.loadFromImage(image);
		texture.setRepeated(true);

		return texture;
	}();

	const int width = m_window.getSize().x;
	const int height = m_window.getSize().y;

	m_filterSprite.setTexture(texture);
	m_filterSprite.setTextureRect({ 0, 0, width, height });
	m_displayFilter = true;
}

void Application::processInput()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_window.close();

		else if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::F2:
				if (m_fpsText.getFont())
					m_displayFps = !m_displayFps;
				break;

			case sf::Keyboard::F3:
				if (m_gridSprite.getTexture())
					m_displayGrid = !m_displayGrid;
				break;

			case sf::Keyboard::F4:
				if (m_filterSprite.getTexture())
					m_displayFilter = !m_displayFilter;
				break;
			}
		}

		else if (event.type == sf::Event::MouseMoved
			  || event.type == sf::Event::MouseButtonPressed
			  || event.type == sf::Event::MouseButtonReleased
			  || event.type == sf::Event::MouseWheelScrolled)
		{
			if (m_displayGrid)
				continue;
		}

		m_stateStack.handleEvent(event);
	}

#ifdef _WIN32
	if (GetAsyncKeyState(VK_SNAPSHOT) & 0x8000)
		takeScreenshot();
#endif
}

void Application::update(sf::Time dt)
{
	m_stateStack.update(dt);
}

void Application::render()
{
	m_window.clear(m_clearColor);
	m_window.draw(m_stateStack);

	if (m_displayFilter)
		m_window.draw(m_filterSprite);
	if (m_displayGrid)
		drawGrid();
	if (m_displayFps)
		m_window.draw(m_fpsText);

	m_window.display();
}

void Application::centerWindow()
{
#ifdef _WIN32
	HWND hWnd = m_window.getSystemHandle();

	RECT rect;
	GetClientRect(hWnd, &rect);

	// NOTE: SFML centers a window by default, but it is not exact

	int left = (GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left)) / 2;
	int top = (GetSystemMetrics(SM_CYSCREEN) - (rect.bottom - rect.top)) / 2;

	const LONG win32Style = GetWindowLong(hWnd, GWL_STYLE);

	if (!(win32Style & WS_POPUP)) // style != sf::Style::None
	{
		// SM_CXSIZEFRAME, SM_CYSIZEFRAME - the thickness of the sizing border around a resizable window
		// SM_CXPADDEDBORDER - the amount of border padding for captioned windows
		// SM_CYCAPTION - the height of the caption area

		left -= GetSystemMetrics(SM_CXSIZEFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
		top -= GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXPADDEDBORDER);
	}

	SetWindowPos(hWnd, nullptr, left, top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	// NOTE: a borderless window (sf::Style::None) works as exclusive fullscreen when the resolution is maximum,
	//       disabling WS_POPUP flag fixes this problem

	SetWindowLong(hWnd, GWL_STYLE, win32Style & ~WS_POPUP);
#endif
}

void Application::takeScreenshot()
{
	const sf::Vector2u windowSize = m_window.getSize();

	sf::Texture texture;
	texture.create(windowSize.x, windowSize.y);
	texture.update(m_window);

	const sf::Image screenshot = texture.copyToImage();

	int fileNumber = 0;

	while (true)
	{
		std::ostringstream oss;
		oss << "screenshot" << std::setfill('0') << std::setw(3) << fileNumber++ << ".png";

		if (!fs::exists(oss.str()))
		{
			screenshot.saveToFile(oss.str());
			break;
		}
	}
}

void Application::countFrame(sf::Time dt)
{
	static sf::Time elapsedTime;
	static std::size_t numFrames = 0;

	elapsedTime += dt;
	numFrames += 1;

	if (elapsedTime >= sf::seconds(1.f))
	{
		m_fpsText.setString("FPS: " + std::to_string(numFrames));
		elapsedTime -= sf::seconds(1.f);
		numFrames = 0;
	}
}

void Application::drawGrid()
{
	m_window.draw(m_gridSprite);

	// HACK: real-time mouse input

	static bool mousePressed = false;
	static sf::Vector2i mousePressedPos;

	sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
	mousePos.x /= m_gridSize.x;
	mousePos.y /= m_gridSize.y;

	const sf::Vector2f rectSize(static_cast<sf::Vector2f>(m_gridSize));

	sf::RectangleShape mouseRect(rectSize);
	mouseRect.setPosition(mousePos.x * rectSize.x, mousePos.y * rectSize.y);
	mouseRect.setFillColor({ 255, 255, 255, 51 });

	sf::Text mouseText(m_fpsText);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (!mousePressed)
		{
			mousePressed = true;
			mousePressedPos = mousePos;
		}

		else
		{
			sf::Vector2i size;
			size.x = std::abs(mousePos.x - mousePressedPos.x) + 1;
			size.y = std::abs(mousePos.y - mousePressedPos.y) + 1;

			sf::Vector2i pos;
			pos.x = std::min(mousePos.x, mousePressedPos.x);
			pos.y = std::min(mousePos.y, mousePressedPos.y);

			mouseRect.setSize({ size.x * rectSize.x, size.y * rectSize.y });
			mouseRect.setPosition(pos.x * rectSize.x, pos.y * rectSize.y);

			mouseText.setString(std::to_string(size.x) + 'x' + std::to_string(size.y));
		}
	}

	else
	{
		mousePressed = false;
		mouseText.setString(std::to_string(mousePos.x) + ',' + std::to_string(mousePos.y));
	}

	const sf::FloatRect bounds = mouseText.getLocalBounds();
	mouseText.setOrigin(0.f, bounds.top + bounds.height);

	const float x = std::clamp((mousePos.x + 1) * rectSize.x, 0.f, m_window.getView().getSize().x - (bounds.left + bounds.width));
	const float y = std::clamp(mousePos.y * rectSize.y, bounds.height, m_window.getView().getSize().y);
	mouseText.setPosition(x, y);

	m_window.draw(mouseRect);
	m_window.draw(mouseText);

	if (mouseText.getGlobalBounds().intersects(m_fpsText.getGlobalBounds()))
		m_fpsText.setString("");
}

}
