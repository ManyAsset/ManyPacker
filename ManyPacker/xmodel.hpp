#pragma once
#include <string>

namespace ManyPacker
{
	namespace XModel
	{
		struct XModel
		{
			std::string name;
			std::vector<std::string> LODs;
		};

		void ReadXModel(std::wstring filepath);
	}
}