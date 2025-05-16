#pragma once
#include "xmodel.hpp"
#include "weapon.hpp"
#include "soundalias.hpp"

namespace ManyPacker
{
	namespace Assets
	{
		extern std::vector<ManyPacker::Weapon::Weapon> Weapons;
		extern std::vector<ManyPacker::XModel::XModel> XModels;
		extern std::vector<std::string> XAnims;
		extern std::vector<ManyPacker::SoundAlias::SoundAlias> SoundAliases;
		extern std::vector<std::string> Materials;
		extern std::vector<std::string> Images;

		extern int exportStatus;

		void ProcessAllAssets();
		void CleanUpAssets();
	}
}