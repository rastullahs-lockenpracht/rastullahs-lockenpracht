/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
	/// Weapons are objects which can be used with combat talents
	class _RlRulesExport Weapon : public Item
	{
	public:
		enum Distanzklasse 
		{
			DK_H = 1,
			DK_N = 2,
			DK_S = 4,
			DK_P = 8
		};

		static const Ogre::Real DK_NAHKAMPF_MIN_DISTANCE;
		static const Ogre::Real DK_STABWAFFEN_MIN_DISTANCE;
		static const Ogre::Real DK_PIKE_MIN_DISTANCE;
		static const Ogre::Real DK_PIKE_MAX_DISTANCE;

        static const Ogre::String CLASS_NAME;

        static const Ogre::String PROPERTY_TP;
        static const Ogre::String PROPERTY_TP_KK;
        static const Ogre::String PROPERTY_TP_BF;
        static const Ogre::String PROPERTY_TP_INI;
        static const Ogre::String PROPERTY_TP_WM;
        static const Ogre::String PROPERTY_TP_DK;
        static const Ogre::String PROPERTY_TP_KAMPFTECHNIK;
        static const Ogre::String PROPERTY_NATURAL;
        static const Ogre::String PROPERTY_AVOID_ARMOR;

		Weapon (const CeGuiString &id);
        virtual ~Weapon(void);

		void setTp(int d6, int d20, int mod);
		const Tripel<int>& getTp() const;
		void setTpKk(int base, int step);
		const std::pair<int, int>& getTpKk() const;
		void setBf(int newBf);
		int getBf() const;
		void setIni(int newIni);
		int getIni() const;
		void setWm(std::pair<int, int>& newWm);
		const std::pair<int,int>& getWm() const;
		void setDk(int newDk);
		int getDk() const;
        bool hasDk(Distanzklasse dk) const;
		void setKampftechnik(const CeGuiString& newKampftechnik);
		const CeGuiString& getKampftechnik() const;
        void setNatural(bool natural);
        bool isNatural() const;
        void setAvoidArmor(bool avoid);
        bool isAvoidingArmor() const;

		/// Return minimum distance in meters where weapon can be used without penalty.
		Ogre::Real getMinimumDistance() const;
		/// Return maximum distance in meters where weapon can be used without penalty.
		Ogre::Real getMaximumDistance() const;

        virtual const Property getProperty(const CeGuiString& key) const;
        virtual void setProperty(const CeGuiString& key, const Property& value);
        virtual PropertyKeys getAllPropertyKeys() const;

	private:
		Tripel<int> mTp;
		std::pair<int, int> mTpKk;
		int mBf;
		int mIni;
		std::pair<int, int> mWm;
		int mDk;
		CeGuiString mKampftechnik;
        bool mNatural;
        bool mAvoidArmor;
	};
}

#endif //__WEAPON_H__
