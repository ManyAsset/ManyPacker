#include "utils.hpp"
#include "xmodel.hpp"
#include "prefs.hpp"
#include <filesystem>
#include <iostream>
#include "material.hpp"

namespace ManyPacker
{
	namespace Assets
	{
		std::vector<ManyPacker::XModel::XModel> XModels;
		std::vector<std::string> Materials;
		std::vector<std::string> Images;

		void ProcessAllAssets()
		{
			if(ManyPacker::Utils::SelectedAssets.empty())
			{
				std::cout << "No assets selected." << std::endl;
				return;
			}

			const std::filesystem::path root = ManyPacker::Prefs::rootfolder;

			for (ManyPacker::Utils::Asset asset : ManyPacker::Utils::SelectedAssets)
			{
				switch (asset.type.type)
				{
				case ManyPacker::Utils::AssetType::XMODEL:
					XModels.push_back({ asset.name });
					ManyPacker::XModel::ReadXModel(root / "raw" / "xmodel" / asset.name);
				}
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

			std::cout << "Images:" << std::endl;
			for (const auto& image : Images)
			{
				std::cout << "  " << image << ".iwi" << std::endl;
			}

			Images.erase(std::remove(Images.begin(), Images.end(), "$identitynormalmap"), Images.end());

			//Make a new directory in ManyPacker::Prefs::outputfolder
            std::filesystem::path outputPath = std::filesystem::path(ManyPacker::Prefs::outputfolder) / "processed_assets";
			std::filesystem::create_directories(outputPath);

			//Create XModel folder if it's more than 0
			if (!XModels.empty())
			{
				std::filesystem::create_directories(outputPath / "xmodel");
				std::filesystem::create_directories(outputPath / "xmodelparts");
				std::filesystem::create_directories(outputPath / "xmodelsurfs");

				for (const auto& xmodel : XModels)
				{
					std::filesystem::copy_file(root / "raw" / "xmodel" / xmodel.name, outputPath / "xmodel" / xmodel.name, std::filesystem::copy_options::overwrite_existing);
					std::filesystem::copy_file(root / "raw" / "xmodelparts" / xmodel.LODs[0], outputPath / "xmodelparts" / xmodel.LODs[0], std::filesystem::copy_options::overwrite_existing);

					for (const auto& lod : xmodel.LODs)
						std::filesystem::copy_file(root / "raw" / "xmodelsurfs" / lod, outputPath / "xmodelsurfs" / lod, std::filesystem::copy_options::overwrite_existing);
				}
			}

			//Create materials folder if it's more than 0
			if (!Materials.empty())
			{
				std::filesystem::create_directories(outputPath / "materials");
				std::filesystem::create_directories(outputPath / "material_properties");
				for (const auto& material : Materials)
				{
					std::filesystem::copy_file(root / "raw" / "materials" / material, outputPath / "materials" / material, std::filesystem::copy_options::overwrite_existing);
					std::filesystem::copy_file(root / "raw" / "material_properties" / material, outputPath / "material_properties" / material, std::filesystem::copy_options::overwrite_existing);
				}
			}

			//Create images folder if it's more than 0
			if (!Images.empty())
			{
				std::filesystem::create_directories(outputPath / "images");
				for (const auto& image : Images)
				{
                    std::string iwi = image + ".iwi";
					std::filesystem::copy_file(root / "raw" / "images" / iwi, outputPath / "images" / iwi, std::filesystem::copy_options::overwrite_existing);
				}
			}

			//Clean up everything
			XModels.clear();
			Materials.clear();
			Images.clear();
		}
	}
}