#include <string>
#include <fstream>
#include <iostream>
#include "utils.hpp"
#include "assets.hpp"

namespace ManyPacker
{
	namespace Material
	{
		void ReadMaterial(std::wstring filepath)
		{
            std::ifstream file(filepath, std::ios::binary);
            if (!file) {
                std::cerr << "Failed to open file.\n";
            }

            file.ignore(48);

            uint16_t numMaps = 0;
            file.read(reinterpret_cast<char*>(&numMaps), sizeof(numMaps));

            file.ignore(14);

            for (int i = 0; i < numMaps; ++i) {
                uint32_t mapTypeOffset = 0;
                uint32_t unused;
                uint32_t mapNameOffset = 0;

                file.read(reinterpret_cast<char*>(&mapTypeOffset), sizeof(mapTypeOffset));
                file.read(reinterpret_cast<char*>(&unused), sizeof(unused));
                file.read(reinterpret_cast<char*>(&mapNameOffset), sizeof(mapNameOffset));

                std::streampos current = file.tellg();

                file.seekg(mapTypeOffset, std::ios::beg);
                std::string mapType = ManyPacker::Utils::ReadNullTerminatedString(file);

                file.seekg(mapNameOffset, std::ios::beg);
                std::string mapName = ManyPacker::Utils::ReadNullTerminatedString(file);

                file.seekg(current);

				ManyPacker::Assets::Images.push_back({ mapName });
            }

            file.close();
		}
	}
}