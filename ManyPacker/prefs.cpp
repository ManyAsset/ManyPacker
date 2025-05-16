#include "prefs.hpp"
#include "utils.hpp"
#include <string>
#include <windows.h>
#include "defines.hpp"
#include <codecvt>

namespace ManyPacker
{
	namespace Prefs
	{
		std::wstring rootfolder;
        char outputfolder[256];
        char outputfolderName[256];

        void LoadPrefs()  
        {  
            rootfolder = loadPreference(L"rootfolder");  
            std::wstring tempOutputFolder = loadPreference(L"outputfolder");

            wcstombs_s(NULL, ManyPacker::Prefs::outputfolder, sizeof(ManyPacker::Prefs::outputfolder), tempOutputFolder.c_str(), _TRUNCATE);

            if(rootfolder.empty())
            {
				//Get the current working directory
				wchar_t currentDir[MAX_PATH];
				GetCurrentDirectoryW(MAX_PATH, currentDir);

                rootfolder = currentDir;

                if (ManyPacker::Utils::checkCod4Dir())
                {
                    savePreference(rootfolder, L"rootfolder");
                }
                else
					rootfolder = L"";
			}
        }

        void savePreference(const std::wstring& value, const LPCWSTR key)
        {
            HKEY hKey;
            if (RegCreateKeyExW(HKEY_CURRENT_USER, MANYPACKER_REG, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS)
            {
                RegSetValueExW(hKey, key, 0, REG_SZ, reinterpret_cast<const BYTE*>(value.c_str()), static_cast<DWORD>((value.size() + 1) * sizeof(wchar_t)));

                RegCloseKey(hKey);
            }
        }

        std::wstring loadPreference(const LPCWSTR key)
        {
            HKEY hKey;
            wchar_t buffer[256];
            DWORD bufferSize = sizeof(buffer);
            std::wstring result;

            if (RegOpenKeyExW(HKEY_CURRENT_USER, MANYPACKER_REG, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
            {
                if (RegQueryValueExW(hKey, key, NULL, NULL, reinterpret_cast<LPBYTE>(buffer), &bufferSize) == ERROR_SUCCESS)
                {
                    result = buffer;
                }
                RegCloseKey(hKey);
            }

            return result;
        }
	}
}