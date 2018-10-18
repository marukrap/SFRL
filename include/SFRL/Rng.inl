namespace rl
{

template <typename T>
const T& Rng::getOne(const std::vector<T>& vector)
{
	assert(!vector.empty());

	const std::size_t size = vector.size();
	const std::size_t i = std::uniform_int_distribution<std::size_t>(0, size - 1)(m_engine);

	return vector[i];
}

template <typename T>
void Rng::shuffle(std::vector<T>& vector)
{
	std::shuffle(vector.begin(), vector.end(), m_engine);
}

}
