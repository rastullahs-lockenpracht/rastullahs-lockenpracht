/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */


#ifndef __ItemDataLoader_H__
#define __ItemDataLoader_H__

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>


#include <string>

namespace rl {

	class Weapon;
	class Armor;

	class ItemDataLoader
	{
	public:
		static void loadData(std::string filename);

	private:
		static void initializeWeapons(XERCES_CPP_NAMESPACE::DOMElement* rootWeapons);
		static Weapon* processWeapon(int gruppe, XERCES_CPP_NAMESPACE::DOMElement* weaponXml);

		static void initializeArmor(XERCES_CPP_NAMESPACE::DOMElement* rootArmors);
		static Armor* processArmor(XERCES_CPP_NAMESPACE::DOMElement* armorXml);

		static int getDKFromString(const std::string& eBeString);
			
		ItemDataLoader();
	};

}

#endif

