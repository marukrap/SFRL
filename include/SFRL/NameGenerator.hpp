#pragma once

// credit: https://github.com/alxgiraud/fantasygen

#include <string>

namespace rl
{

enum class Species
{
	Goblin,    // neuter
	Orc,       // neuter
	Ogre,      // neuter
	Caveman,
	Dwarf,
	Gnome,
	Halfling,
	Faerykind, // alternative(High fairy)
	Elf,       // alternative(High elf)
	DarkElf,   // alternative(Drow)
	HalfDemon,
	Demon,     // neuter
	Angel,
	Dragon,
};

enum class Gender
{
	Male,
	Female,
};

// generic fantasy name
std::string getFantasyName();

// fantastic species
std::string getFantasyName(Species species, Gender gender = Gender::Male, bool alternative = false);

// groups
std::string getMysticOrderName();
std::string getMilitaryUnitName();
std::string getThievesAndAssassinsName();

// taverns
std::string getTavernName();

}
