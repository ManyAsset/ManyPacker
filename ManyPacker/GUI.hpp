#pragma once
#include <string>

namespace ManyPacker
{
	namespace GUI
	{
		void DrawGUI();
		void SetStyle();
	}

	namespace Components
	{
		void AboutWindow();
		void ItemSelection(std::string name);
	}
}