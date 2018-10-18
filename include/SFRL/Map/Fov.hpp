#pragma once

// credit: http://journal.stuffwithstuff.com/2015/09/07/what-the-hero-sees/

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <vector>

namespace rl
{

class Map;

// field of view
class Fov : public sf::Drawable, public sf::Transformable
{
public:
	struct Shadow
	{
		bool contains(const Shadow& projection) const;

		float start, end;
	};

public:
	Fov() = default;
	Fov(const sf::Vector2i& tileSize, const sf::Texture* texture = nullptr, int tileBegin = 0);

	Fov(const Fov&) = delete;
	Fov& operator=(const Fov&) = delete;

	void setTexture(const sf::Vector2i& tileSize, const sf::Texture* texture = nullptr, int tileBegin = 0);
	void setMap(Map& map);
	void setViewRect(const sf::IntRect& rect);

	void clear();
	void compute(const sf::Vector2i& position, int range);

private:
	void refreshOctant(int octant, const sf::Vector2i& start, int range);

	Shadow getProjection(int col, int row);
	bool isInShadow(const Shadow& projection) const;
	bool addShadow(const Shadow& shadow);

	bool isVisible(int x, int y) const;
	bool isExplored(int x, int y) const;

	void appendQuad(int x, int y, int tileOffset, const sf::Color& color = sf::Color::White) const;
	void updateVertices() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	const sf::Texture* m_texture = nullptr;
	sf::Vector2i m_tileSize;
	int m_tileBegin = 0;
	Map* m_map = nullptr;
	sf::IntRect m_viewRect;
	std::vector<Shadow> m_shadows;
	mutable std::vector<sf::Vertex> m_vertices;
	mutable bool m_verticesNeedUpdate = false;
};

}

