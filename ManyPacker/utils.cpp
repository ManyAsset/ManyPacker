#include <string>
#include <combaseapi.h>
#include <ShObjIdl.h>
#include "prefs.hpp"
#include <filesystem>
#include <iostream>
#include "utils.hpp"
#include <fstream>


namespace ManyPacker
{
	namespace Utils
	{
        std::vector<std::string> XModels;
		std::vector<std::string> MPWeapons;
		std::vector<std::string> SPWeapons;

        std::vector<Asset> SelectedAssets;
        std::vector<Asset> AdditionalAssets;

        std::wstring getExeDirectory()
        {
            wchar_t path[MAX_PATH];
            GetModuleFileNameW(NULL, path, MAX_PATH);
            std::filesystem::path exePath(path);
            return exePath.parent_path().wstring();
        }

        std::wstring selectFolder()
        {
            std::wstring folderPath;
            HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
            if (SUCCEEDED(hr))
            {
                IFileDialog* pFileDialog = nullptr;
                hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
                if (SUCCEEDED(hr))
                {
                    DWORD options;
                    pFileDialog->GetOptions(&options);
                    pFileDialog->SetOptions(options | FOS_PICKFOLDERS);

                    if (SUCCEEDED(pFileDialog->Show(NULL)))
                    {
                        IShellItem* pItem;
                        if (SUCCEEDED(pFileDialog->GetResult(&pItem)))
                        {
                            PWSTR path = nullptr;
                            if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &path)))
                            {
                                folderPath = path;
                                CoTaskMemFree(path);
                            }
                            pItem->Release();
                        }
                    }
                    pFileDialog->Release();
                }

                CoUninitialize();
            }

			return folderPath;
        }

        void selectCoD4Root()
        {
			std::wstring selectedFolder = selectFolder();

            if (!selectedFolder.empty())
                ManyPacker::Prefs::rootfolder = selectedFolder;

            if (ManyPacker::Prefs::rootfolder.empty())
                exit(0);

			ManyPacker::Prefs::savePreference(ManyPacker::Prefs::rootfolder, L"rootfolder");
            updateAssets();
        }

        bool checkCod4Dir()
        {
            const std::filesystem::path root = ManyPacker::Prefs::rootfolder;

            std::filesystem::path xmodelPath = root / "raw" / "xmodel";
            std::filesystem::path weaponsMpPath = root / "raw" / "weapons" / "mp";
            std::filesystem::path weaponsSpPath = root / "raw" / "weapons" / "sp";


            return std::filesystem::exists(xmodelPath) && std::filesystem::is_directory(xmodelPath) &&
                std::filesystem::exists(weaponsMpPath) && std::filesystem::is_directory(weaponsMpPath) &&
                std::filesystem::exists(weaponsSpPath) && std::filesystem::is_directory(weaponsSpPath);
        }

        void updateAssets()
        {
            if (ManyPacker::Prefs::rootfolder.empty() || !checkCod4Dir())
                return;

			XModels.clear();
			MPWeapons.clear();
            SPWeapons.clear();

            const std::filesystem::path root = ManyPacker::Prefs::rootfolder;
			std::filesystem::path xmodelPath = root / "raw" / "xmodel";
			std::filesystem::path weaponsMpPath = root / "raw" / "weapons" / "mp";
			std::filesystem::path weaponsSpPath = root / "raw" / "weapons" / "sp";

            for (const auto& entry : std::filesystem::directory_iterator(xmodelPath))
            {
                if (entry.is_regular_file() && entry.path().extension() == "")
                {
                    std::string xmodelName = entry.path().filename().string();
                    XModels.push_back(xmodelName);
                }
            }

            for (const auto& entry : std::filesystem::directory_iterator(weaponsMpPath))
            {
                if (entry.is_regular_file() && entry.path().extension() == "")
                {
                    std::string weaponName = entry.path().filename().string();
                    MPWeapons.push_back(weaponName);
                }
			}

            for (const auto& entry : std::filesystem::directory_iterator(weaponsSpPath))
            {
                if (entry.is_regular_file() && entry.path().extension() == "")
                {
                    std::string weaponName = entry.path().filename().string();
                    SPWeapons.push_back(weaponName);
                }
            }
        }

        void addAsset(std::string assetname, ManyPacker::Utils::AssetType::Type type)
        {
			auto it = std::find_if(SelectedAssets.begin(), SelectedAssets.end(), [&](const Asset& asset) {
				return asset.name == assetname && asset.type.type == type;
			});

			if (it == SelectedAssets.end())
				SelectedAssets.push_back({ assetname, type });

        }

        void removeAsset(int index)
        {
            if (index < 0 || index >= static_cast<int>(SelectedAssets.size()))
                return;

            SelectedAssets.erase(SelectedAssets.begin() + index);
        }

        std::string ReadNullTerminatedString(std::ifstream& file)
        {
            std::string result;
            char c;
            while (file.get(c) && c != '\0') {
                result += c;
            }
            return result;
        }
    }
}