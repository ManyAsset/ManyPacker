#pragma once
#include <string>
#include <vector>
#include "utils.hpp"

namespace ManyPacker
{
	namespace GUI
	{
		void DrawGUI();
		void SetStyle();
	}

	namespace Components
	{
		void AboutWindow();
		void SelectDirWindow();
		void InvalidDirWindow();

		void SuccessfulExportWindow();
		void ErrorExportWindow();
		void AssetListEmptyWindow();

		void ItemSelection(std::string name, std::vector<std::string>& items, ManyPacker::Utils::AssetType::Type type);
	}
}