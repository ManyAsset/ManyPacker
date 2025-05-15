#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "assets.hpp"

std::string ReadNullTerminatedString(std::ifstream& file)
{
    std::string result;
    char c;
    while (file.get(c) && c != '\0') {
        result += c;
    }
    return result;
}

namespace ManyPacker
{
    namespace XModel
    {
        void ReadXModel(std::wstring filepath)
        {
            std::ifstream file(filepath, std::ios::binary);
            if (!file)
            {
                std::cerr << "Failed to open file.\n";
            }

            uint16_t version;
            file.read(reinterpret_cast<char*>(&version), 2);
            if (file.gcount() != 2 || (version != 25))
            {
                std::cerr << "Invalid or unsupported XModel version.\n";
            }

            file.seekg(25, std::ios::cur);
            ReadNullTerminatedString(file);
            
            struct LOD {
                std::string name;
                std::vector<std::string> materials;
            };

            std::vector<LOD> lods;
            for (int i = 0; i < 4; ++i)
            {
                uint32_t someInt;
                file.read(reinterpret_cast<char*>(&someInt), 4);
                std::string lodName = ReadNullTerminatedString(file);
                if (!lodName.empty()) {
                    lods.push_back({ lodName });
                }
            }

            if (lods.empty())
            {
                std::cerr << "No LODs found in model.\n";
            }

			for (const auto& lod : lods)
			{
				ManyPacker::Assets::XModels.back().LODs.push_back(lod.name);
			}

            // Skip section
            file.seekg(4, std::ios::cur);
            uint32_t count;
            file.read(reinterpret_cast<char*>(&count), 4);

            for (uint32_t i = 0; i < count; ++i)
            {
                uint32_t subcount;
                file.read(reinterpret_cast<char*>(&subcount), 4);
                file.seekg((subcount * 48) + 36, std::ios::cur);
            }

            // Read materials for the first LOD
            uint16_t numMaterials;
            file.read(reinterpret_cast<char*>(&numMaterials), 2);

            for (int i = 0; i < numMaterials; ++i)
            {
                ManyPacker::Assets::Materials.push_back(ReadNullTerminatedString(file));
            }
        }
    }
}