#include "utils.hpp"
#include "xmodel.hpp"
#include "prefs.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include "material.hpp"
#include "weapon.hpp"
#include "assets.hpp"

namespace ManyPacker
{
	namespace Assets
	{
		std::vector<ManyPacker::Weapon::Weapon> Weapons;
		std::vector<ManyPacker::XModel::XModel> XModels;
		std::vector<std::string> Materials;
		std::vector<std::string> Images;

		int exportStatus = -1;

		void ProcessAllAssets()
		{
			if(ManyPacker::Utils::SelectedAssets.empty())
			{
				std::cout << "No assets selected." << std::endl;
				exportStatus = 0;
				return;
			}

			const std::filesystem::path root = ManyPacker::Prefs::rootfolder;

			for (ManyPacker::Utils::Asset asset : ManyPacker::Utils::SelectedAssets)
			{
				switch (asset.type.type)
				{
					case ManyPacker::Utils::AssetType::SP_WEAPON:
						Weapons.push_back({ asset.name, ManyPacker::Weapon::Weapon::SP_WEAPON });
						ManyPacker::Weapon::ReadWeapon(root / "raw" / "weapons" / "sp" / asset.name);
						break;
					case ManyPacker::Utils::AssetType::MP_WEAPON:
						Weapons.push_back({ asset.name, ManyPacker::Weapon::Weapon::MP_WEAPON });
						ManyPacker::Weapon::ReadWeapon(root / "raw" / "weapons" / "mp" / asset.name);
						break;
					case ManyPacker::Utils::AssetType::XMODEL:
						XModels.push_back({ asset.name });
						ManyPacker::XModel::ReadXModel(root / "raw" / "xmodel" / asset.name);
						break;
				}
			}

			std::cout << "Weapons:" << std::endl;
			for (const auto& weapon : Weapons)
			{
				std::cout << "  " << weapon.name << std::endl;
			}

			std::cout << "XModels:" << std::endl;
			for (const auto& xmodel : XModels)
			{
				std::cout << xmodel.name << std::endl;

				for (const auto& lod : xmodel.LODs)
				{
					std::cout << "  LOD: " << lod << std::endl;
				}
			}

			std::sort(Materials.begin(), Materials.end());
			Materials.erase(std::unique(Materials.begin(), Materials.end()), Materials.end());
			Materials.erase(std::remove(Materials.begin(), Materials.end(), "lambert1"), Materials.end());

			std::cout << "Materials:" << std::endl;
			for (const auto& material : Materials)
			{
				std::cout << "  " << material << std::endl;
			}

			//Load all images
			for (const auto& material : Materials)
				ManyPacker::Material::ReadMaterial(root / "raw" / "materials" / material);

			Images.erase(std::remove_if(Images.begin(), Images.end(), [](const std::string& image) { return image[0] == '$'; }), Images.end());

			std::cout << "Images:" << std::endl;
			for (const auto& image : Images)
			{
				std::cout << "  " << image << ".iwi" << std::endl;
			}

			//Make a new directory in ManyPacker::Prefs::outputfolder

			std::string outputFolder = ManyPacker::Prefs::outputfolderName[0] == '\0' ? "processed_assets" : ManyPacker::Prefs::outputfolderName;

            std::filesystem::path outputPath = std::filesystem::path(ManyPacker::Prefs::outputfolder) / outputFolder;
			std::filesystem::create_directories(outputPath);

			// Assume exportStatus is defined somewhere above this snippet

			// Create XModel folder if it's more than 0
			if (!XModels.empty())
			{
				std::filesystem::create_directories(outputPath / "xmodel");
				std::filesystem::create_directories(outputPath / "xmodelparts");
				std::filesystem::create_directories(outputPath / "xmodelsurfs");

				for (const auto& xmodel : XModels)
				{
					auto xmodelPath = root / "raw" / "xmodel" / xmodel.name;
					auto partPath = root / "raw" / "xmodelparts" / xmodel.LODs[0];

					if (!std::filesystem::exists(xmodelPath))
					{
						std::cout << "Error: Missing file " << xmodelPath << '\n';
						exportStatus = 2;
						CleanUpAssets();
						return;
					}

					if (!std::filesystem::exists(partPath))
					{
						std::cout << "Error: Missing file " << partPath << '\n';
						exportStatus = 2;
						CleanUpAssets();
						return;
					}

					std::filesystem::copy_file(xmodelPath, outputPath / "xmodel" / xmodel.name, std::filesystem::copy_options::overwrite_existing);
					std::filesystem::copy_file(partPath, outputPath / "xmodelparts" / xmodel.LODs[0], std::filesystem::copy_options::overwrite_existing);

					for (const auto& lod : xmodel.LODs)
					{
						auto surfPath = root / "raw" / "xmodelsurfs" / lod;

						if (!std::filesystem::exists(surfPath))
						{
							std::cout << "Error: Missing file " << surfPath << '\n';
							exportStatus = 2;
							CleanUpAssets();
							return;
						}
						std::filesystem::copy_file(surfPath, outputPath / "xmodelsurfs" / lod, std::filesystem::copy_options::overwrite_existing);
					}
				}
			}

			// Create materials folder if it's more than 0
			if (!Materials.empty())
			{
				std::filesystem::create_directories(outputPath / "materials");
				std::filesystem::create_directories(outputPath / "material_properties");

				for (const auto& material : Materials)
				{
					auto matPath = root / "raw" / "materials" / material;
					auto propPath = root / "raw" / "material_properties" / material;

					if (!std::filesystem::exists(matPath))
					{
						std::cout << "Error: Missing file " << matPath << '\n';
						exportStatus = 2;
						CleanUpAssets();
						return;
					}
					if (!std::filesystem::exists(propPath))
					{
						std::cout << "Error: Missing file " << propPath << '\n';
						exportStatus = 2;
						CleanUpAssets();
						return;
					}

					std::filesystem::copy_file(matPath, outputPath / "materials" / material, std::filesystem::copy_options::overwrite_existing);
					std::filesystem::copy_file(propPath, outputPath / "material_properties" / material, std::filesystem::copy_options::overwrite_existing);
				}
			}

			// Create images folder if it's more than 0
			if (!Images.empty())
			{
				std::filesystem::create_directories(outputPath / "images");

				for (const auto& image : Images)
				{
					std::string iwi = image + ".iwi";
					auto imagePath = root / "raw" / "images" / iwi;

					if (!std::filesystem::exists(imagePath))
					{
						std::cout << "Error: Missing file " << imagePath << '\n';
						exportStatus = 2;
						CleanUpAssets();
						return;
					}

					std::filesystem::copy_file(imagePath, outputPath / "images" / iwi, std::filesystem::copy_options::overwrite_existing);
				}
			}

			// Create weapons folder if it's more than 0
			if (!Weapons.empty())
			{
				std::filesystem::create_directories(outputPath / "weapons" );
				for (const auto& weapon : Weapons)
				{
					std::filesystem::create_directories(outputPath / "weapons" / (weapon.type == ManyPacker::Weapon::Weapon::SP_WEAPON ? "sp" : "mp"));
					auto weaponPath = root / "raw" / "weapons" / (weapon.type == ManyPacker::Weapon::Weapon::SP_WEAPON ? "sp" : "mp") / weapon.name;
					if (!std::filesystem::exists(weaponPath))
					{
						std::cout << "Error: Missing file " << weaponPath << '\n';
						exportStatus = 2;
						CleanUpAssets();
						return;
					}
					std::filesystem::copy_file(weaponPath, outputPath / "weapons" / (weapon.type == ManyPacker::Weapon::Weapon::SP_WEAPON ? "sp" : "mp") / weapon.name, std::filesystem::copy_options::overwrite_existing);
				}
			}

			//Create the mod.csv file
			std::ofstream modcsv(outputPath / "mod.csv");
			if (modcsv.is_open())
			{
				for (const auto& asset : ManyPacker::Utils::SelectedAssets)
				{
					modcsv << const_cast<ManyPacker::Utils::AssetType&>(asset.type).toString() << asset.name << std::endl;
				}
			}

			modcsv.close();

			CleanUpAssets();

			exportStatus = 1;
		}

		void Assets::CleanUpAssets()
		{

			Weapons.clear();
			XModels.clear();
			Materials.clear();
			Images.clear();
		}
	}
}