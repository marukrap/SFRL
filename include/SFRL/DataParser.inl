namespace rl
{

template <typename T>
T DataParser::get() const
{
	assert(!m_value.empty());

	T output;
	toValue(m_value, output);

	return output;
}

template <typename T>
T DataParser::get(const T& defaultValue) const
{
	if (m_value.empty())
		return defaultValue;

	return get<T>();
}

template <typename... Args>
void DataParser::get(Args&... args) const
{
	std::istringstream iss(m_value);
	(toValue(iss, args), ...);
}

template <typename T>
void DataParser::get(std::vector<T>& vector) const
{
	std::istringstream iss(m_value);

	for (std::string token; std::getline(iss, token, ','); )
	{
		vector.emplace_back();
		toValue(trim(token), vector.back());
	}
}

template <typename T>
void DataParser::toValue(const std::string& input, T& output) const
{
	std::istringstream iss(input);
	iss >> std::boolalpha >> output;
}

template <>
inline void DataParser::toValue(const std::string& input, std::string& output) const
{
	// TODO: multiline text

	if (input.size() >= 2 && input.front() == '\"' && input.back() == '\"')
		output = input.substr(1, input.size() - 2);
	else
		output = input;
}

template <>
inline void DataParser::toValue(const std::string& input, std::wstring& output) const
{
	std::string str;
	toValue(input, str);
	output = ansiToUtf16(str);
}

template <>
inline void DataParser::toValue(const std::string& input, sf::Color& output) const
{
	std::istringstream iss(input);
	std::string token;

	output.r = std::getline(iss, token, ',') ? std::stoi(token) : 0;
	output.g = std::getline(iss, token, ',') ? std::stoi(token) : 0;
	output.b = std::getline(iss, token, ',') ? std::stoi(token) : 0;
	output.a = std::getline(iss, token, ',') ? std::stoi(token) : 255;
}

template <>
inline void DataParser::toValue(const std::string& input, sf::Vector2i& output) const
{
	std::istringstream iss(input);
	std::string token;

	output.x = std::getline(iss, token, ',') ? std::stoi(token) : 0;
	output.y = std::getline(iss, token, ',') ? std::stoi(token) : 0;
}

template <typename T>
void DataParser::toValue(std::istringstream& iss, T& output) const
{
	if (std::string token; std::getline(iss, token, ','))
		toValue(token, output);
}

template <>
inline void DataParser::toValue(std::istringstream& iss, std::string& output) const
{
	if (std::string token; std::getline(iss, token, ','))
	{
		const std::string str = trim(token);

		if (!str.empty() && str.front() == '\"' && (str.size() == 1 || str.back() != '\"'))
		{
			output = token.substr(token.find_first_not_of(" \t\n")) + ',';

			if (std::getline(iss, token, '\"'))
				output += token + '\"';

			if (std::getline(iss, token, ','))
				output += token; // optional
		}

		else
			toValue(str, output);
	}
}

template <>
inline void DataParser::toValue(std::istringstream& iss, std::wstring& output) const
{
	std::string str;
	toValue(iss, str);
	output = ansiToUtf16(str);
}

}
