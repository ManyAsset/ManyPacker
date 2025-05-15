#pragma once
#include <string>
#include <windows.h>

namespace ManyPacker
{
	namespace Prefs
	{
		extern std::wstring rootfolder;
		extern char outputfolder[256];

		void LoadPrefs();
		void savePreference(const std::wstring& value, const LPCWSTR key);
		std::wstring loadPreference(const LPCWSTR key);
	}
}