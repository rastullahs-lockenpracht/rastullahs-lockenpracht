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

		Weapon (const CeGuiString& weaponName, const CeGuiString& description);
		virtual ~Weapon(void);

		void setTp(int d6, int d20, int mod);
		const Tripel<int>& getTp() const;
		void setTpKk(int base, int step);
		const pair<int, int>& getTpKk() const;
		void setBf(int newBf);
		int getBf();
		void setIni(int newIni);
		int getIni();
		void setWm(pair<int, int>& newWm);
		const pair<int,int>& getWm() const;
		void setDk(Distanzklasse newDk);
		Distanzklasse getDk();
		void setKampftechnik(const CeGuiString& newKampftechnik);
		const CeGuiString& getKampftechnik() const;

		/// Ermittelt, wie weit die Entfernung von der optimalen Distanzklasse entfernt ist
		/// @param distance die Entfernung von Angreifer und Verteidiger
		/// @return Anzahl der Distanzklassen, die von der optimalen DK trennen (positiv = zu weit entfernt)
		const int getDkDistanceToOptimum(Ogre::Real distance) const;

	private:
		Tripel<int> mTp;
		pair<int, int> mTpKk;
		int mBf;
		int mIni;
		pair<int, int> mWm;
		Distanzklasse mDk;
		CeGuiString mKampftechnik;
	};
}

#endif //__WEAPON_H__
