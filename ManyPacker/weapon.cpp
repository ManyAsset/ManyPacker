#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "assets.hpp"
#include <filesystem>
#include "prefs.hpp"

namespace ManyPacker
{
	namespace Weapon
	{
		void ReadWeapon(std::wstring filepath)
		{
			std::ifstream file(filepath, std::ios::binary);
			if (!file)
			{
				std::cerr << "Failed to open file.\n";
			}

			std::stringstream buffer;
			buffer << file.rdbuf();
			std::string data = buffer.str();

			// Tokenize by backslash
			std::vector<std::string> tokens;
			size_t pos = 0;

			while (pos < data.size())
			{
				size_t next = data.find('\\', pos);
				if (next == std::string::npos)
					next = data.size();

				std::string token = data.substr(pos, next - pos);
				tokens.push_back(token);

				pos = next + 1;
			}

			// Validate starting token
			if (tokens.empty() || tokens[0] != "WEAPONFILE") {
				std::cerr << "Invalid weapon file.\n";
				return;
			}

			// Process tokens
			std::unordered_map<std::string, std::string> weaponData;
			for (size_t i = 1; i + 1 < tokens.size(); i += 2)
			{
				weaponData[tokens[i]] = tokens[i + 1];
			}
			
			const std::filesystem::path root = ManyPacker::Prefs::rootfolder;

			//gunModel and worldModel
			for (size_t i = 0; i < 16; i++)
			{
				std::string gun = "gunModel" + (i != 1 ? std::to_string(i) : "");
				std::string world = "worldModel" + (i != 1 ? std::to_string(i) : "");

				if( weaponData.find(gun) != weaponData.end())
				{
					if (!weaponData[gun].empty())
					{
						ManyPacker::Assets::XModels.push_back({ weaponData[gun] });
						ManyPacker::XModel::ReadXModel(root / "raw" / "xmodel" / weaponData[gun]);
					}
				}

				if( weaponData.find(world) != weaponData.end())
				{
					if (!weaponData[world].empty())
					{
						ManyPacker::Assets::XModels.push_back({ weaponData[world] });
						ManyPacker::XModel::ReadXModel(root / "raw" / "xmodel" / weaponData[world]);
					}
				}
			}

			//knifeModel and worldKnifeModel
			if( weaponData.find("knifeModel") != weaponData.end())
			{
				if (!weaponData["knifeModel"].empty() && weaponData["knifeModel"] != "viewmodel_knife")
				{
					ManyPacker::Assets::XModels.push_back({ weaponData["knifeModel"] });
					ManyPacker::XModel::ReadXModel(root / "raw" / "xmodel" / weaponData["knifeModel"]);
				}
			}

			if( weaponData.find("worldKnifeModel") != weaponData.end())
			{
				if (!weaponData["worldKnifeModel"].empty() && weaponData["worldKnifeModel"] != "weapon_parabolic_knife")
				{
					ManyPacker::Assets::XModels.push_back({ weaponData["worldKnifeModel"] });
					ManyPacker::XModel::ReadXModel(root / "raw" / "xmodel" / weaponData["worldKnifeModel"]);
				}
			}

			//XAnims
			const std::vector<std::string> animKeys = {
			"idleAnim", "emptyIdleAnim", "fireAnim", "lastShotAnim", "rechamberAnim",
			"meleeAnim", "meleeChargeAnim", "reloadAnim", "reloadEmptyAnim",
			"reloadStartAnim", "reloadEndAnim", "raiseAnim", "dropAnim", "firstRaiseAnim",
			"altRaiseAnim", "altDropAnim", "quickRaiseAnim", "quickDropAnim",
			"emptyRaiseAnim", "emptyDropAnim", "sprintInAnim", "sprintLoopAnim",
			"sprintOutAnim", "nightVisionWearAnim", "nightVisionRemoveAnim",
			"adsFireAnim", "adsLastShotAnim", "adsRechamberAnim", "adsUpAnim", "adsDownAnim"
			};

			const std::vector<std::string> ignoredXanims = {
			"viewmodel_M4m203_knife_melee_1", "viewmodel_M4m203_knife_melee_2"
			};

			for (const auto& key : animKeys)
			{
				if (weaponData.find(key) != weaponData.end())
				{
					if (!weaponData[key].empty())
					{
						bool ignored = false;
						for (const auto& ignoredAnim : ignoredXanims)
						{
							if (weaponData[key] == ignoredAnim)
							{
								ignored = true;
								break;
							}
						}

						if (!ignored)
							ManyPacker::Assets::XAnims.push_back({ weaponData[key] });
					}
				}
			}

			file.close();
		}
	}
}