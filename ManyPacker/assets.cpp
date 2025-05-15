#include "utils.hpp"
#include "xmodel.hpp"
#include "prefs.hpp"
#include <filesystem>
#include <iostream>

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
				std::cout << material << std::endl;
			}

			//Clean up everything
			XModels.clear();
			Materials.clear();
			Images.clear();
		}
	}
}