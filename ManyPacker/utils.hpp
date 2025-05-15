#pragma once
#include <vector>
#include <string>

namespace ManyPacker
{
	namespace Utils
	{
		extern std::vector<std::string> XModels;
		extern std::vector<std::string> MPWeapons;
		extern std::vector<std::string> SPWeapons;

		class AssetType
		{
			public:
				enum Type
				{
					MP_WEAPON,
					SP_WEAPON,
					XMODEL,
					UNKNOWN
				};

			Type type;

			std::string toString()
			{
				switch (type)
				{
					case MP_WEAPON:return "weapon,mp/";
					case SP_WEAPON: return "weapon,sp/";
					case XMODEL: return "xmodel,";
					default: return "Unknown";
				}
			}
		};

		struct Asset
		{
			std::string name;
			AssetType type;
		};

		extern std::vector<Asset> SelectedAssets;

		std::wstring getExeDirectory();
		std::wstring selectFolder();
		void selectCoD4Root();
		bool checkCod4Dir();
		void updateAssets();

		void addAsset(std::string assetname, ManyPacker::Utils::AssetType::Type type);
		void removeAsset(int index);
	}
}