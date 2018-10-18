#pragma once

#include "Utility.hpp"

#include <SFML/Graphics/Color.hpp>

#include <sstream>
#include <unordered_map>
#include <memory>
#include <cassert>

namespace rl
{

class DataParser
{
public:
	DataParser() = default;
	explicit DataParser(const std::string& filename);

	void load(const std::string& filename);

	DataParser(const DataParser&) = delete;
	DataParser& operator=(const DataParser&) = delete;

	DataParser& operator[](const std::string& key);
	const DataParser& operator[](const std::string& key) const;

	DataParser& at(const std::string& key);
	const DataParser& at(const std::string& key) const;

	DataParser* find(const std::string& key) const;

	std::vector<std::pair<std::string, DataParser*>> getChildren() const;

	template <typename T>
	T get() const;
	template <typename T>
	T get(const T& defaultValue) const;
	template <typename... Args>
	void get(Args&... args) const;
	template <typename T>
	void get(std::vector<T>& vector) const;

private:
	template <typename T>
	void toValue(const std::string& input, T& output) const;
	template <typename T>
	void toValue(std::istringstream& iss, T& output) const;

	std::pair<std::string, std::string> parseLine(const std::string& line);

private:
	int m_indent = -1;
	std::string m_value;
	std::unordered_map<std::string, std::unique_ptr<DataParser>> m_children;
};

}

#include "DataParser.inl"
