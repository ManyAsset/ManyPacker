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

			const std::vector<std::string> ignoredXModelSuffix = {
				"_brock", "_bshdwl", "_bwmrpt", "_cmdtgr", "_stagger"
			};

			const std::vector<std::string> ignoredXModels = {
				"viewmodel_winchester1200_mp_brock", "viewmodel_winchester1200_mp_bshdwl", "viewmodel_winchester1200_mp_bwmrpt",
				"viewmodel_winchester1200_mp_cmdtgr", "viewmodel_winchester1200_mp_stagger", "viewmodel_ak47",
				"viewmodel_ak47_and_silencer_mp", "viewmodel_ak47_and_silencer_mp_brock", "viewmodel_ak47_and_silencer_mp_bshdwl",
				"viewmodel_ak47_and_silencer_mp_bwmrpt", "viewmodel_ak47_and_silencer_mp_cmdtgr", "viewmodel_ak47_and_silencer_mp_stagger",
				"viewmodel_ak47_gold_and_silencer_mp", "viewmodel_ak47_gold_grenadier_mp", "viewmodel_ak47_gold_mp",
				"viewmodel_ak47_grenadier", "viewmodel_ak47_grenadier_mp", "viewmodel_ak47_grenadier_mp_brock",
				"viewmodel_ak47_grenadier_mp_bshdwl", "viewmodel_ak47_grenadier_mp_bwmrpt", "viewmodel_ak47_grenadier_mp_cmdtgr",
				"viewmodel_ak47_grenadier_mp_stagger", "viewmodel_ak47_mp", "viewmodel_ak47_mp_brock",
				"viewmodel_ak47_mp_bshdwl", "viewmodel_ak47_mp_bwmrpt", "viewmodel_ak47_mp_cmdtgr",
				"viewmodel_ak47_mp_stagger", "viewmodel_ak47_silencer", "viewmodel_ak74u",
				"viewmodel_ak74u_mp", "viewmodel_ak74u_mp_brock", "viewmodel_ak74u_mp_bshdwl",
				"viewmodel_ak74u_mp_bwmrpt", "viewmodel_ak74u_mp_cmdtgr", "viewmodel_ak74u_mp_stagger",
				"viewmodel_ak74u_silencer", "viewmodel_ak74usd_mp_brock", "viewmodel_ak74usd_mp_bshdwl",
				"viewmodel_ak74usd_mp_bwmrpt", "viewmodel_ak74usd_mp_cmdtgr", "viewmodel_ak74usd_mp_stagger",
				"viewmodel_at4", "viewmodel_barrett", "viewmodel_barrett_mp",
				"viewmodel_barrett_mp_brock", "viewmodel_barrett_mp_bshdwl", "viewmodel_barrett_mp_bwmrpt",
				"viewmodel_barrett_mp_cmdtgr", "viewmodel_barrett_mp_stagger", "viewmodel_base_character",
				"viewmodel_base_fastrope_character", "viewmodel_base_viewhands", "viewmodel_benelli_m4",
				"viewmodel_benelli_m4_gold_mp", "viewmodel_benelli_m4_mp", "viewmodel_benelli_m4_mp_brock",
				"viewmodel_benelli_m4_mp_bshdwl", "viewmodel_benelli_m4_mp_bwmrpt", "viewmodel_benelli_m4_mp_cmdtgr",
				"viewmodel_benelli_m4_mp_stagger", "viewmodel_beretta", "viewmodel_beretta_and_silencer_mp",
				"viewmodel_beretta_mp", "viewmodel_binoculars", "viewmodel_briefcase_bomb_mp",
				"viewmodel_c4", "viewmodel_claymore", "viewmodel_colt45",
				"viewmodel_colt45_and_silencer_mp", "viewmodel_colt45_mp", "viewmodel_default",
				"viewmodel_desert_eagle_gold_mp", "viewmodel_dragunov", "viewmodel_dragunov_gold_mp",
				"viewmodel_dragunov_mp", "viewmodel_dragunov_mp_brock", "viewmodel_dragunov_mp_bshdwl",
				"viewmodel_dragunov_mp_bwmrpt", "viewmodel_dragunov_mp_cmdtgr", "viewmodel_dragunov_mp_stagger",
				"viewmodel_g3", "viewmodel_g3_and_silencer_mp_brock", "viewmodel_g3_and_silencer_mp_bshdwl",
				"viewmodel_g3_and_silencer_mp_cmdtgr", "viewmodel_g3_and_silencer_mp_stagger", "viewmodel_g3_and_sliencer_mp_bwmrpt",
				"viewmodel_g3_gl_mp_brock", "viewmodel_g3_gl_mp_bshdwl", "viewmodel_g3_gl_mp_bwmrpt",
				"viewmodel_g3_gl_mp_cmdtgr", "viewmodel_g3_gl_mp_stagger", "viewmodel_g3_mp",
				"viewmodel_g3_mp_brock", "viewmodel_g3_mp_bshdwl", "viewmodel_g3_mp_bwmrpt",
				"viewmodel_g3_mp_cmdtgr", "viewmodel_g3_mp_stagger", "viewmodel_g3_silencer",
				"viewmodel_g3m203_mp", "viewmodel_g36c", "viewmodel_g36c_gl_mp_brock",
				"viewmodel_g36c_gl_mp_bshdwl", "viewmodel_g36c_gl_mp_bwmrpt", "viewmodel_g36c_gl_mp_cmdtgr",
				"viewmodel_g36c_gl_mp_stagger", "viewmodel_g36c_mp", "viewmodel_g36c_mp_brock",
				"viewmodel_g36c_mp_bshdwl", "viewmodel_g36c_mp_bwmrpt", "viewmodel_g36c_mp_cmdtgr",
				"viewmodel_g36c_mp_stagger", "viewmodel_g36c_silencer", "viewmodel_g36cm203_mp",
				"viewmodel_g36csd_mp_brock", "viewmodel_g36csd_mp_bshdwl", "viewmodel_g36csd_mp_bwmrpt",
				"viewmodel_g36csd_mp_cmdtgr", "viewmodel_g36csd_mp_stagger", "viewmodel_gasmask",
				"viewmodel_hands_cloth", "viewmodel_javelin", "viewmodel_knife",
				"viewmodel_m2_50cal", "viewmodel_m4", "viewmodel_m4_acog",
				"viewmodel_m4_and_silencer", "viewmodel_m4_and_silencer_mp", "viewmodel_m4_and_silencer_mp_brock",
				"viewmodel_m4_and_silencer_mp_bshdwl", "viewmodel_m4_and_silencer_mp_bwmrpt", "viewmodel_m4_and_silencer_mp_cmdtgr",
				"viewmodel_m4_and_silencer_mp_stagger", "viewmodel_m4_mp", "viewmodel_m4_mp_brock",
				"viewmodel_m4_mp_bshdwl", "viewmodel_m4_mp_bwmrpt", "viewmodel_m4_mp_cmdtgr",
				"viewmodel_m4_mp_stagger", "viewmodel_m4_silencer_acog", "viewmodel_m4m203",
				"viewmodel_m4m203_acog", "viewmodel_m4m203_and_silencer", "viewmodel_m4m203_mp",
				"viewmodel_m4m203_mp_brock", "viewmodel_m4m203_mp_bshdwl", "viewmodel_m4m203_mp_bwmrpt",
				"viewmodel_m4m203_mp_cmdtgr", "viewmodel_m4m203_mp_stagger", "viewmodel_m4m203_silencer_reflex",
				"viewmodel_m14", "viewmodel_m14_gl_mp", "viewmodel_m14_mp",
				"viewmodel_m14_mp_brock", "viewmodel_m14_mp_bshdwl", "viewmodel_m14_mp_bwmrpt",
				"viewmodel_m14_mp_cmdtgr", "viewmodel_m14_mp_stagger", "viewmodel_m14_scout_gl_mp_brock",
				"viewmodel_m14_scout_gl_mp_bshdwl", "viewmodel_m14_scout_gl_mp_bwmrpt", "viewmodel_m14_scout_gl_mp_cmdtgr",
				"viewmodel_m14_scout_gl_mp_stagger", "viewmodel_m14_woodland", "viewmodel_m14sd_mp",
				"viewmodel_m14sd_mp_brock", "viewmodel_m14sd_mp_bshdwl", "viewmodel_m14sd_mp_bwmrpt",
				"viewmodel_m14sd_mp_cmdtgr", "viewmodel_m14sd_mp_stagger", "viewmodel_m16_and_silencer_mp",
				"viewmodel_m16_and_silencer_mp_brock", "viewmodel_m16_and_silencer_mp_bshdwl", "viewmodel_m16_and_silencer_mp_bwmrpt",
				"viewmodel_m16_and_silencer_mp_cmdtgr", "viewmodel_m16_and_silencer_mp_stagger", "viewmodel_m16_mp",
				"viewmodel_m16_mp_brock", "viewmodel_m16_mp_bshdwl", "viewmodel_m16_mp_bwmrpt",
				"viewmodel_m16_mp_cmdtgr", "viewmodel_m16_mp_stagger", "viewmodel_m16m203",
				"viewmodel_m16m203_mp", "viewmodel_m16m203_mp_brock", "viewmodel_m16m203_mp_bshdwl",
				"viewmodel_m16m203_mp_bwmrpt", "viewmodel_m16m203_mp_cmdtgr", "viewmodel_m16m203_mp_stagger",
				"viewmodel_m21_mp", "viewmodel_m21_mp_brock", "viewmodel_m21_mp_bshdwl",
				"viewmodel_m21_mp_bwmrpt", "viewmodel_m21_mp_cmdtgr", "viewmodel_m21_mp_stagger",
				"viewmodel_m21sd", "viewmodel_m40a3", "viewmodel_m40a3_camo",
				"viewmodel_m40a3_mp", "viewmodel_m40a3_mp_brock", "viewmodel_m40a3_mp_bshdwl",
				"viewmodel_m40a3_mp_bwmrpt", "viewmodel_m40a3_mp_cmdtgr", "viewmodel_m40a3_mp_stagger",
				"viewmodel_m60", "viewmodel_m60_gold_mp", "viewmodel_m60_mp",
				"viewmodel_m60_mp_brock", "viewmodel_m60_mp_bshdwl", "viewmodel_m60_mp_bwmrpt",
				"viewmodel_m60_mp_cmdtgr", "viewmodel_m60_mp_stagger", "viewmodel_m67",
				"viewmodel_m84", "viewmodel_m249", "viewmodel_m249_mp",
				"viewmodel_m249_mp_brock", "viewmodel_m249_mp_bshdwl", "viewmodel_m249_mp_bwmrpt",
				"viewmodel_m249_mp_cmdtgr", "viewmodel_m249_mp_stagger", "viewmodel_minigun",
				"viewmodel_miniuzi", "viewmodel_miniuzi_gold_mp", "viewmodel_miniuzi_mp",
				"viewmodel_miniuzi_mp_brock", "viewmodel_miniuzi_mp_bshdwl", "viewmodel_miniuzi_mp_bwmrpt",
				"viewmodel_miniuzi_mp_cmdtgr", "viewmodel_miniuzi_mp_stagger", "viewmodel_miniuzi_silencer",
				"viewmodel_miniuzi_supressed_gold_mp", "viewmodel_miniuzi_supressed_mp", "viewmodel_miniuzi_supressed_mp_brock",
				"viewmodel_miniuzi_supressed_mp_bshdwl", "viewmodel_miniuzi_supressed_mp_bwmrpt", "viewmodel_miniuzi_supressed_mp_cmdtgr",
				"viewmodel_miniuzi_supressed_mp_stagger", "viewmodel_mk2", "viewmodel_mk19_agl_shield",
				"viewmodel_mk19_agl_tripod", "viewmodel_mp5", "viewmodel_mp5_mp",
				"viewmodel_mp5_mp_brock", "viewmodel_mp5_mp_bshdwl", "viewmodel_mp5_mp_bwmrpt",
				"viewmodel_mp5_mp_cmdtgr", "viewmodel_mp5_mp_stagger", "viewmodel_mp5_silencer",
				"viewmodel_mp5_silencer_mp", "viewmodel_mp5_silencer_mp_brock", "viewmodel_mp5_silencer_mp_bshdwl",
				"viewmodel_mp5_silencer_mp_bwmrpt", "viewmodel_mp5_silencer_mp_cmdtgr", "viewmodel_mp5_silencer_mp_stagger",
				"viewmodel_mp5_silencer_reflex", "viewmodel_mp44", "viewmodel_mp44_mp",
				"viewmodel_mp44_mp_brock", "viewmodel_mp44_mp_bshdwl", "viewmodel_mp44_mp_bwmrpt",
				"viewmodel_mp44_mp_cmdtgr", "viewmodel_mp44_mp_stagger", "viewmodel_nvg",
				"viewmodel_p90", "viewmodel_p90_mp", "viewmodel_p90_mp_brock",
				"viewmodel_p90_mp_bshdwl", "viewmodel_p90_mp_bwmrpt", "viewmodel_p90_mp_cmdtgr",
				"viewmodel_p90_mp_stagger", "viewmodel_p90_silencer", "viewmodel_p90_silencer_mp",
				"viewmodel_p90_silencer_mp_brock", "viewmodel_p90_silencer_mp_bshdwl", "viewmodel_p90_silencer_mp_bwmrpt",
				"viewmodel_p90_silencer_mp_cmdtgr", "viewmodel_p90_silencer_mp_stagger", "viewmodel_remington700",
				"viewmodel_remington700_mp", "viewmodel_remington700_mp_brock", "viewmodel_remington700_mp_bshdwl",
				"viewmodel_remington700_mp_bwmrpt", "viewmodel_remington700_mp_cmdtgr", "viewmodel_remington700_mp_stagger",
				"viewmodel_rpd", "viewmodel_rpd_mp", "viewmodel_rpd_mp_brock",
				"viewmodel_rpd_mp_bshdwl", "viewmodel_rpd_mp_bwmrpt", "viewmodel_rpd_mp_cmdtgr",
				"viewmodel_rpd_mp_stagger", "viewmodel_rpg7", "viewmodel_rpg7_rocket",
				"viewmodel_skorpion", "viewmodel_skorpion_mp", "viewmodel_skorpion_mp_brock",
				"viewmodel_skorpion_mp_bshdwl", "viewmodel_skorpion_mp_bwmrpt", "viewmodel_skorpion_mp_cmdtgr",
				"viewmodel_skorpion_mp_stagger", "viewmodel_skorpion_silencer", "viewmodel_skorpionsd_mp_brock",
				"viewmodel_skorpionsd_mp_bshdwl", "viewmodel_skorpionsd_mp_bwmrpt", "viewmodel_skorpionsd_mp_cmdtgr",
				"viewmodel_skorpionsd_mp_stagger", "viewmodel_stinger", "viewmodel_usp",
				"viewmodel_usp_mp", "viewmodel_usp_silencer", "viewmodel_usp_silencer_mp",
				"viewmodel_ussmokegrenade", "viewmodel_winchester1200", "viewmodel_winchester1200_mp",
			};

			//gunModel and worldModel
			for (size_t i = 0; i < 16; i++)
			{
				std::string gun = "gunModel" + (i != 1 ? std::to_string(i) : "");
				std::string world = "worldModel" + (i != 1 ? std::to_string(i) : "");

				if (weaponData.find(gun) != weaponData.end() && !weaponData[gun].empty())
				{
					bool ignored = false;
					for (const auto& suffix : ignoredXModelSuffix)
					{
						if (weaponData[gun].ends_with(suffix))
						{
							ignored = true;
							break;
						}
					}

					for (const auto& ignoredXmodel : ignoredXModels)
					{
						if (weaponData[gun] == ignoredXmodel)
						{
							ignored = true;
							break;
						}
					}

					if (ignored)
						continue;

					ManyPacker::Utils::AdditionalAssets.push_back({ weaponData[gun], ManyPacker::Utils::AssetType::XMODEL });
					ManyPacker::Assets::XModels.push_back({ weaponData[gun] });
					ManyPacker::XModel::ReadXModel(root / "raw" / "xmodel" / weaponData[gun]);
				}

				if( weaponData.find(world) != weaponData.end() && !weaponData[world].empty())
				{
					bool ignored = false;
					for (const auto& suffix : ignoredXModelSuffix)
					{
						if (weaponData[world].ends_with(suffix))
						{
							ignored = true;
							break;
						}
					}

					for (const auto& ignoredXmodel : ignoredXModels)
					{
						if (weaponData[world] == ignoredXmodel)
						{
							ignored = true;
							break;
						}
					}

					if (ignored)
						continue;

					ManyPacker::Utils::AdditionalAssets.push_back({ weaponData[world], ManyPacker::Utils::AssetType::XMODEL });
					ManyPacker::Assets::XModels.push_back({ weaponData[world] });
					ManyPacker::XModel::ReadXModel(root / "raw" / "xmodel" / weaponData[world]);
				}
			}

			//knifeModel and worldKnifeModel
			if( weaponData.find("knifeModel") != weaponData.end() && !weaponData["knifeModel"].empty() && weaponData["knifeModel"] != "viewmodel_knife")
			{
				ManyPacker::Utils::AdditionalAssets.push_back({ weaponData["knifeModel"], ManyPacker::Utils::AssetType::XMODEL });
				ManyPacker::Assets::XModels.push_back({ weaponData["knifeModel"] });
				ManyPacker::XModel::ReadXModel(root / "raw" / "xmodel" / weaponData["knifeModel"]);
			}

			if( weaponData.find("worldKnifeModel") != weaponData.end() && !weaponData["worldKnifeModel"].empty() && weaponData["worldKnifeModel"] != "weapon_parabolic_knife")
			{
				ManyPacker::Utils::AdditionalAssets.push_back({ weaponData["worldKnifeModel"], ManyPacker::Utils::AssetType::XMODEL });
				ManyPacker::Assets::XModels.push_back({ weaponData["worldKnifeModel"] });
				ManyPacker::XModel::ReadXModel(root / "raw" / "xmodel" / weaponData["worldKnifeModel"]);
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
				"killIcon_44","killIcon_30cal","killIcon_40mm_grenade","killIcon_40mm_grenade_mp", "killIcon_ak47"
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