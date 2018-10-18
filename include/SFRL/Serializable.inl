namespace rl
{

template <typename T>
void Serializable::serialize(std::ostream& os, const T& target)
{
	os.write(reinterpret_cast<const char*>(&target), sizeof(target));
}

template <>
inline void Serializable::serialize(std::ostream& os, const std::string& target)
{
	const std::size_t size = target.size();
	serialize(os, size);

	for (std::size_t i = 0; i < size; ++i)
		serialize(os, target[i]);
}

template <>
inline void Serializable::serialize(std::ostream& os, const std::wstring& target)
{
	const std::size_t size = target.size();
	serialize(os, size);

	for (std::size_t i = 0; i < size; ++i)
		serialize(os, target[i]);
}

template <typename T>
void Serializable::serialize(std::ostream& os, const std::vector<T>& target)
{
	const std::size_t size = target.size();
	serialize(os, size);

	for (std::size_t i = 0; i < size; ++i)
		serialize(os, target[i]);
}

template <typename T>
void Serializable::deserialize(std::istream& is, T& target)
{
	is.read(reinterpret_cast<char*>(&target), sizeof(target));
}

template <>
inline void Serializable::deserialize(std::istream& is, std::string& target)
{
	std::size_t size = 0;
	deserialize(is, size);

	target.resize(size);

	for (std::size_t i = 0; i < size; ++i)
		deserialize(is, target[i]);
}

template <>
inline void Serializable::deserialize(std::istream& is, std::wstring& target)
{
	std::size_t size = 0;
	deserialize(is, size);

	target.resize(size);

	for (std::size_t i = 0; i < size; ++i)
		deserialize(is, target[i]);
}

template <typename T>
void Serializable::deserialize(std::istream& is, std::vector<T>& target)
{
	std::size_t size = 0;
	deserialize(is, size);

	target.resize(size);

	for (std::size_t i = 0; i < size; ++i)
		deserialize(is, target[i]);
}

}
