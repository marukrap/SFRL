#include "DataParser.hpp"

#include <fstream>
#include <stack>
// #include <cctype>

namespace rl
{

DataParser::DataParser(const std::string& filename)
{
	load(filename);
}

void DataParser::load(const std::string& filename)
{
	std::ifstream ifs(filename);

	if (!ifs)
		throw std::runtime_error("Failed to open file \"" + filename + "\".");

	std::stack<DataParser*> stack;
	stack.push(this);

	for (std::string line; std::getline(ifs, line); )
	{
		// remove comment
		if (const auto found = line.find("//"); found != std::string::npos)
			line = line.substr(0, found);

		const auto [key, value] = parseLine(line);

		if (key.empty())
			continue;

		int indent = 0;

		for (auto ch : line)
		{
			if (ch == ' ')
				indent += 1;
			else if (ch == '\t')
				indent += 4;
			else
				break;
		}

		auto child = std::make_unique<DataParser>();
		child->m_indent = indent;
		child->m_value = value;

		while (stack.top()->m_indent >= indent)
			stack.pop();

		const auto inserted = stack.top()->m_children.emplace(key, std::move(child));
		assert(inserted.second);

		stack.emplace(inserted.first->second.get());
	}
}

DataParser& DataParser::operator[](const std::string& key)
{
	const auto found = m_children.find(key);

	if (found != m_children.end())
		return *found->second;

	auto child = std::make_unique<DataParser>();
	child->m_indent = m_indent + 4;

	DataParser& result = *child;
	m_children[key] = std::move(child);

	return result;
}

const DataParser& DataParser::operator[](const std::string& key) const
{
	const auto found = m_children.find(key);
	assert(found != m_children.end());

	return *found->second;
}

DataParser& DataParser::at(const std::string& key)
{
	const auto found = m_children.find(key);
	assert(found != m_children.end());

	return *found->second;
}

const DataParser& DataParser::at(const std::string& key) const
{
	const auto found = m_children.find(key);
	assert(found != m_children.end());

	return *found->second;
}

DataParser* DataParser::find(const std::string& key) const
{
	const auto found = m_children.find(key);

	if (found != m_children.end())
		return found->second.get();

	return nullptr;
}

std::vector<std::pair<std::string, DataParser*>> DataParser::getChildren() const
{
	std::vector<std::pair<std::string, DataParser*>> results;

	for (const auto& [key, child] : m_children)
		results.emplace_back(key, child.get());

	return results;
}

std::pair<std::string, std::string> DataParser::parseLine(const std::string& line)
{
	std::string key, value;
	std::string str = trim(line);

	if (!str.empty()) // && !std::ispunct(str[0]))
	{
		const std::size_t equal = str.find('=');

		if (equal != std::string::npos)
		{
			key = str.substr(0, equal);
			value = str.substr(equal + 1);

			key = trim(key);
			value = trim(value);
		}

		else
			key = str;
	}

	return { key, value };
}

}
