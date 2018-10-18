#pragma once

#include <fstream>
#include <vector>
#include <string>

namespace rl
{

class Serializable
{
public:
	virtual ~Serializable() = default;

	virtual void save(std::ostream& os) = 0; // const?
	virtual void load(std::istream& is) = 0;

protected:
	template <typename T>
	static void serialize(std::ostream& os, const T& target);
	template <typename T>
	static void serialize(std::ostream& os, const std::vector<T>& target);
	template <typename T>
	static void deserialize(std::istream& is, T& target);
	template <typename T>
	static void deserialize(std::istream& is, std::vector<T>& target);
};

}

#include "Serializable.inl"
