#pragma once
#include "xmodel.hpp"

namespace ManyPacker
{
	namespace Assets
	{

		extern std::vector<ManyPacker::XModel::XModel> XModels;
		extern std::vector<std::string> Materials;
		extern std::vector<std::string> Images;

		void ProcessAllAssets();
	}
}