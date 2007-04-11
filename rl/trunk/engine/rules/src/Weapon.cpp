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

#include "DsaManager.h"
#include "Weapon.h"

using namespace std;

namespace rl
{
    const Ogre::String Weapon::CLASS_NAME = "Weapon";

	Weapon::Weapon(unsigned int id)
		: Item(id)
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

	int Weapon::getBf()
	{
		return mBf;
	}

	void Weapon::setIni(int newIni)
	{
		mIni = newIni;
	}

	int Weapon::getIni()
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

	void Weapon::setDk(Weapon::Distanzklasse newDk)
	{
		mDk = newDk;
	}

	Weapon::Distanzklasse Weapon::getDk()
	{
		return mDk;
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
        //if (key == Weapon::PROPERTY_IMAGENAME)
        //{
        //    mImageName = value.toString();
        //}
        //else
        //{
            Item::setProperty(key, value);
        //}
    }

    const Property Weapon::getProperty(const Ogre::String &key) const
    {
        //if (key == Item::PROPERTY_IMAGENAME)
        //{
        //    return Property(mImageName);
        //}
        //else
        //{
            return Item::getProperty(key);
        //}
    }

    PropertySet* Weapon::getAllProperties() const
    {
        PropertySet* ps = Item::getAllProperties();
        //ps->setProperty(Item::PROPERTY_IMAGENAME, Property(mImageName));
        //ps->setProperty(Item::PROPERTY_SIZE, Property(mSize));

        return ps;
    }

}

