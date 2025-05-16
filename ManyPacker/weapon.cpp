#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "assets.hpp"
#include <filesystem>
#include "prefs.hpp"
#include "utils.hpp"

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

				if (weaponData.find(gun) != weaponData.end())
				{
					if (!weaponData[gun].empty())
					{
						ManyPacker::Utils::AdditionalAssets.push_back({ weaponData[gun], ManyPacker::Utils::AssetType::XMODEL });
						ManyPacker::Assets::XModels.push_back({ weaponData[gun] });
						ManyPacker::XModel::ReadXModel(root / "raw" / "xmodel" / weaponData[gun]);
					}
				}

				if( weaponData.find(world) != weaponData.end())
				{
					if (!weaponData[world].empty())
					{
						ManyPacker::Utils::AdditionalAssets.push_back({ weaponData[world], ManyPacker::Utils::AssetType::XMODEL });
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
					ManyPacker::Utils::AdditionalAssets.push_back({ weaponData["knifeModel"], ManyPacker::Utils::AssetType::XMODEL });
					ManyPacker::Assets::XModels.push_back({ weaponData["knifeModel"] });
					ManyPacker::XModel::ReadXModel(root / "raw" / "xmodel" / weaponData["knifeModel"]);
				}
			}

			if( weaponData.find("worldKnifeModel") != weaponData.end())
			{
				if (!weaponData["worldKnifeModel"].empty() && weaponData["worldKnifeModel"] != "weapon_parabolic_knife")
				{
					ManyPacker::Utils::AdditionalAssets.push_back({ weaponData["worldKnifeModel"], ManyPacker::Utils::AssetType::XMODEL });
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

			//Materials
			const std::vector<std::string> materialKeys = {
				"killIcon", "hudIcon", "adsOverlayShader", "adsOverlayShaderLowRes"
			};

			const std::vector<std::string> ignoredMaterials = {
				"hud_icon_ak47_gp25", "hud_icon_ak74u", "hud_icon_artillery",
				"hud_icon_at4", "hud_icon_barrett50cal", "hud_icon_benelli_m4",
				"hud_icon_c4", "hud_icon_claymore", "hud_icon_cobra",
				"hud_icon_colt_45", "hud_icon_desert_eagle", "hud_icon_dragunov",
				"hud_icon_g3", "hud_icon_g36c", "hud_icon_g36c_mp",
				"hud_icon_javelin", "hud_icon_m4_grenadier", "hud_icon_m4_grunt",
				"hud_icon_m4_silencer", "hud_icon_m4carbine", "hud_icon_m4m203_silencer",
				"hud_icon_m9beretta", "hud_icon_m14", "hud_icon_m14_scoped",
				"hud_icon_m16a4", "hud_icon_m16a4_grenade", "hud_icon_m40a3",
				"hud_icon_m60e4", "hud_icon_m249saw", "hud_icon_m249saw_mounted",
				"hud_icon_mini_uzi", "hud_icon_minigun", "hud_icon_mp5",
				"hud_icon_mp5_silencer", "hud_icon_mp44", "hud_icon_nvg",
				"hud_icon_p90", "hud_icon_pistol", "hud_icon_remington700",
				"hud_icon_rpd", "hud_icon_rpg", "hud_icon_rpg_dpad",
				"hud_icon_shotgun", "hud_icon_skorpian", "hud_icon_sniperrifle",
				"hud_icon_stinger", "hud_icon_usp_45", "hud_icon_winchester_1200",
				"killIcon_44","killIcon_30cal","killIcon_40mm_grenade","killIcon_40mm_grenade_mp","killIcon_ak47"
			};

			for (const auto& key : materialKeys)
			{
				if (weaponData.find(key) != weaponData.end())
				{
					if (!weaponData[key].empty())
					{
						bool ignored = false;
						for (const auto& ignoredMaterial : ignoredMaterials)
						{
							if (weaponData[key] == ignoredMaterial)
							{
								ignored = true;
								break;
							}
						}

						if (!ignored)
						{
							ManyPacker::Utils::AdditionalAssets.push_back({ weaponData[key], ManyPacker::Utils::AssetType::MATERIAL });
							ManyPacker::Assets::Materials.push_back({ weaponData[key] });
						}
					}
				}
			}

			file.close();
		}
	}
}