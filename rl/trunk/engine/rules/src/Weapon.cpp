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
#include "stdinc.h" //precompiled header

#include "DsaManager.h"
#include "Weapon.h"

using namespace std;

namespace rl
{
    const Ogre::String Weapon::CLASS_NAME = "Weapon";
    const Ogre::String Weapon::PROPERTY_TP = "TP";
    const Ogre::String Weapon::PROPERTY_TP_KK = "TPKK";
    const Ogre::String Weapon::PROPERTY_TP_BF = "BF";
    const Ogre::String Weapon::PROPERTY_TP_INI = "INI";
    const Ogre::String Weapon::PROPERTY_TP_WM = "WM";
    const Ogre::String Weapon::PROPERTY_TP_DK = "DK";
    const Ogre::String Weapon::PROPERTY_TP_KAMPFTECHNIK = "KT";

	Weapon::Weapon(unsigned int id)
		: Item(id),
        mTp(0, 0, 0),
		mTpKk(0, 0),
		mBf(0),
		mIni(0),
		mWm(0, 0),
        mDk(DK_H),
        mKampftechnik("")
	{
        mQueryFlags |= QUERYFLAG_WEAPON;
	}

	Weapon::~Weapon()
	{
	}


	void Weapon::setTp(int d6, int d20, int mod)
	{
		mTp.first = d6;
		mTp.second = d20;
		mTp.third = mod;
	}

	const Tripel<int>& Weapon::getTp() const
	{
		return mTp;
	}

	void Weapon::setTpKk(int base, int step)
	{
		mTpKk.first = base;
		mTpKk.second = step;
	}

	const pair<int, int>& Weapon::getTpKk() const
	{
		return mTpKk;
	}

	void Weapon::setBf(int newBf)
	{
		mBf = newBf;
	}

	int Weapon::getBf() const
	{
		return mBf;
	}

	void Weapon::setIni(int newIni)
	{
		mIni = newIni;
	}

	int Weapon::getIni() const
	{
		return mIni;
	}

	void Weapon::setWm(pair<int, int>& newWm)
	{
		mWm = newWm;
	}

	const pair<int, int>& Weapon::getWm() const
	{
		return mWm;
	}

	void Weapon::setDk(int newDk)
	{
		mDk = newDk;
	}

	int Weapon::getDk() const
	{
		return mDk;
	}

    bool Weapon::hasDk(Weapon::Distanzklasse newDk) const
    {
        return (mDk & newDk) == newDk;
    }

	void Weapon::setKampftechnik(const CeGuiString newKampftechnik)
	{
		mKampftechnik = newKampftechnik;
	}

	const CeGuiString Weapon::getKampftechnik() const
	{
		return mKampftechnik;
	}

    void Weapon::setProperty(const Ogre::String &key, const rl::Property &value)
    {
        if (key == Weapon::PROPERTY_TP)
        {
            mTp = value.toIntTriple();
        }
        else if (key == Weapon::PROPERTY_TP_KK)
        {
            mTpKk = value.toIntPair();
        }
        else if (key == Weapon::PROPERTY_TP_BF)
        {
            mBf = value.toInt();
        }
        else if (key == Weapon::PROPERTY_TP_INI)
        {
            mIni = value.toInt();
        }
        else if (key == Weapon::PROPERTY_TP_WM)
        {
            mWm = value.toIntPair();
        }
        else if (key == Weapon::PROPERTY_TP_DK)
        {
            mDk = value.toInt();
        }
        else if (key == Weapon::PROPERTY_TP_KAMPFTECHNIK)
        {
            mKampftechnik = value.toString();
        }
        else
        {
            Item::setProperty(key, value);
        }
    }

    const Property Weapon::getProperty(const Ogre::String &key) const
    {
        if (key == Weapon::PROPERTY_TP)
        {
            return Property(mTp);
        }
        else if (key == Weapon::PROPERTY_TP_KK)
        {
            return Property(mTpKk);
        }
        else if (key == Weapon::PROPERTY_TP_BF)
        {
            return Property(mBf);
        }
        else if (key == Weapon::PROPERTY_TP_INI)
        {
            return Property(mIni);
        }
        else if (key == Weapon::PROPERTY_TP_WM)
        {
            return Property(mWm);
        }
        else if (key == Weapon::PROPERTY_TP_DK)
        {
            return Property(mDk);
        }
        else if (key == Weapon::PROPERTY_TP_KAMPFTECHNIK)
        {
            return Property(mKampftechnik);
        }
        else
        {
            return Item::getProperty(key);
        }
    }

    PropertySet* Weapon::getAllProperties() const
    {
        PropertySet* ps = Item::getAllProperties();
        ps->setProperty(Weapon::PROPERTY_TP, Property(mTp));
        ps->setProperty(Weapon::PROPERTY_TP_KK, Property(mTpKk));
        ps->setProperty(Weapon::PROPERTY_TP_BF, Property(mBf));
        ps->setProperty(Weapon::PROPERTY_TP_INI, Property(mIni));
        ps->setProperty(Weapon::PROPERTY_TP_WM, Property(mWm));
        ps->setProperty(Weapon::PROPERTY_TP_DK, Property(mDk));
        ps->setProperty(Weapon::PROPERTY_TP_KAMPFTECHNIK, Property(mKampftechnik));

        return ps;
    }
}
