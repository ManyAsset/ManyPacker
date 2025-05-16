#include <string>
#include "rapidcsv.h"
#include "assets.hpp"

namespace ManyPacker
{
	namespace SoundAlias
	{
		void ReadSoundAlias(std::wstring filepath)
		{
			std::ifstream file(filepath, std::ios::binary);
			if (!file)
			{
				std::cerr << "Failed to open file.\n";
			}

			std::stringstream sstream;
			sstream << file.rdbuf();

			std::stringstream cleaned;
			std::string line;
			bool firstLine = true;

			while (std::getline(sstream, line)) {
				if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
					continue;
				}

				if (!firstLine) {
					cleaned << '\n';
				}
				firstLine = false;

				cleaned << line;
			}

			sstream.str(cleaned.str());
			sstream.clear();

			try
			{
				rapidcsv::Document doc(sstream);

				ManyPacker::Assets::SoundAliases.back().files = doc.GetColumn<std::string>("file");
			}
			catch (const std::exception& e)
			{
				std::cerr << "Error reading sound alias file: " << filepath.c_str() << std::endl;
				return;
			}

			file.close();
		}
	}
}