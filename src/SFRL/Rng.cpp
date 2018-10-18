#include "Rng.hpp"

#include <iostream>

namespace rl
{

Rng::Rng(unsigned int seed)
	: m_seed(seed)
	, m_engine(seed)
{
}

int Rng::getSeed() const
{
	return m_seed;
}

void Rng::printSeed() const
{
	std::cout << "RNG Seed: 0x" << std::hex << m_seed << std::dec << '\n';
}

int Rng::getInt(int exclusiveMax)
{
	assert(exclusiveMax > 0);

	return std::uniform_int_distribution<>(0, exclusiveMax - 1)(m_engine);
}

int Rng::getInt(int min, int inclusiveMax)
{
	assert(min <= inclusiveMax);

	return min + std::uniform_int_distribution<>(0, inclusiveMax - min)(m_engine);
}

int Rng::rollDice(int number, int sides)
{
	assert(number > 0 && sides > 0);

	int result = 0;

	for (int i = 0; i < number; ++i)
		result += getInt(1, sides);

	return result;
}

int Rng::rollDiceHighest(int number, int sides, int highest)
{
	assert(number > 0 && sides > 0 && highest <= number);

	std::vector<int> vector;

	for (int i = 0; i < number; ++i)
		vector.emplace_back(getInt(1, sides));

	std::sort(vector.begin(), vector.end(), std::greater<int>());

	int result = 0;

	for (int i = 0; i < highest; ++i)
		result += vector[i];

	return result;
}

int Rng::rollDiceLowest(int number, int sides, int lowest)
{
	assert(number > 0 && sides > 0 && lowest <= number);

	std::vector<int> vector;

	for (int i = 0; i < number; ++i)
		vector.emplace_back(getInt(1, sides));

	std::sort(vector.begin(), vector.end());

	int result = 0;

	for (int i = 0; i < lowest; ++i)
		result += vector[i];

	return result;
}

float Rng::getFloat(float max)
{
	assert(max > 0.f);

	return std::uniform_real_distribution<float>(0.f, max)(m_engine);
}

float Rng::getFloat(float min, float max)
{
	assert(min < max);

	return std::uniform_real_distribution<float>(min, max)(m_engine);
}

bool Rng::getBool(double probability)
{
	assert(probability >= 0.0);

	return std::bernoulli_distribution(probability)(m_engine);
}

sf::Vector2i Rng::getPoint(const sf::Vector2i& size)
{
	return { getInt(size.x), getInt(size.y) };
}

sf::Vector2i Rng::getPoint(const sf::IntRect& bounds)
{
	return { bounds.left + getInt(bounds.width), bounds.top + getInt(bounds.height) };
}

sf::Color Rng::getColor()
{
	return sf::Color((getInt(0, 0xffffff) << 8) | 0xff);
}

}
