#pragma once
#include <string>
namespace ManyPacker
{
	namespace Weapon
	{
		struct Weapon
		{
			std::string name;

			enum Type
			{
				MP_WEAPON,
				SP_WEAPON,
			} type;
		};

		void ReadWeapon(std::wstring filepath);
	}
}