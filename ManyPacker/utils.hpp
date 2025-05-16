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
					MATERIAL,
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
					case MATERIAL: return "material,";
					default: return "Unknown";
				}
			}
		};

		class Asset
		{
			public:
				std::string name;
				AssetType type;

				bool operator==(const Asset& other) const
				{
					return name == other.name;
				}

				bool operator<(const Asset& other) const
				{
					return name < other.name;
				}
		};

		extern std::vector<Asset> SelectedAssets;
		extern std::vector<Asset> AdditionalAssets;
		extern bool useSoundAliases;

		std::wstring getExeDirectory();
		std::wstring selectFolder();
		void selectCoD4Root();
		bool checkCod4Dir();
		void updateAssets();

		void addAsset(std::string assetname, ManyPacker::Utils::AssetType::Type type);
		void removeAsset(int index);

		std::string ReadNullTerminatedString(std::ifstream& file);
	}
}