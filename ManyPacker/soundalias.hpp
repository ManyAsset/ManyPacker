#pragma once
#include <string>
#include <vector>

namespace ManyPacker
{
	namespace SoundAlias
	{
		struct SoundAlias
		{
			std::string name;
			std::vector<std::string> files;
		};

		void ReadSoundAlias(std::wstring filepath);
	}
}