#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>

#include <random>
#include <cassert>

namespace rl
{

// random number generator
class Rng
{
public:
	explicit Rng(unsigned int seed = std::random_device()());

	int getSeed() const;
	void printSeed() const;

	int getInt(int exclusiveMax);          // [0, max)
	int getInt(int min, int inclusiveMax); // [min, max]

	int rollDice(int number, int sides);   // roll s sided dice n times
	int rollDiceHighest(int number, int sides, int highest);
	int rollDiceLowest(int number, int sides, int lowest);

	float getFloat(float max = 1.f);       // [0, max)
	float getFloat(float min, float max);  // [min, max)
	bool getBool(double probability = 0.5);

	sf::Vector2i getPoint(const sf::Vector2i& size);
	sf::Vector2i getPoint(const sf::IntRect& bounds);
	sf::Color getColor();

	template <typename T>
	const T& getOne(const std::vector<T>& vector);
	template <typename T>
	void shuffle(std::vector<T>& vector);

private:
	unsigned int m_seed;
	std::mt19937 m_engine;
};

}

#include "Rng.inl"
