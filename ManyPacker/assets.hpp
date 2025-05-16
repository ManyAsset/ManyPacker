#pragma once
#include "xmodel.hpp"
#include "weapon.hpp"

namespace ManyPacker
{
	namespace Assets
	{
		extern std::vector<ManyPacker::Weapon::Weapon> Weapons;
		extern std::vector<ManyPacker::XModel::XModel> XModels;
		extern std::vector<std::string> Materials;
		extern std::vector<std::string> Images;

		extern int exportStatus;

		void ProcessAllAssets();
	}
}