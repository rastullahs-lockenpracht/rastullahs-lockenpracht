/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Item.h"
#include "Tripel.h"

namespace rl
{
	/// Objekte die sich mit Kampffertigkeiten einsetzen lassen
	class _RlRulesExport Weapon : public Item
	{
	public:
		enum Distanzklasse 
		{
			DK_H=0,
			DK_N,
			DK_S,
			DK_P
		};

        static const Ogre::String CLASS_NAME;

		Weapon (unsigned int id);
        virtual ~Weapon(void);

		const CeGuiString getDescription() const;

		void setTp(int d6, int d20, int mod);
		const Tripel<int>& getTp() const;
		void setTpKk(int base, int step);
		const std::pair<int, int>& getTpKk() const;
		void setBf(int newBf);
		int getBf();
		void setIni(int newIni);
		int getIni();
		void setWm(std::pair<int, int>& newWm);
		const std::pair<int,int>& getWm() const;
		void setDk(Distanzklasse newDk);
		Distanzklasse getDk();
		void setKampftechnik(const CeGuiString newKampftechnik);
		const CeGuiString getKampftechnik() const;

        virtual const Property getProperty(const Ogre::String& key) const;
        virtual void setProperty(const Ogre::String& key, const Property& value);
        virtual PropertySet* getAllProperties() const;

	private:
		Tripel<int> mTp;
		std::pair<int, int> mTpKk;
		int mBf;
		int mIni;
		std::pair<int, int> mWm;
		Distanzklasse mDk;
		CeGuiString mKampftechnik;
	};
}

#endif //__WEAPON_H__
