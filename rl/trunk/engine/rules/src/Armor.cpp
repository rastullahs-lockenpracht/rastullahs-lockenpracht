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
#include "Armor.h"

using namespace std;

namespace rl
{
    const Ogre::String Armor::CLASS_NAME = "Armor";

    const Ogre::String Armor::PROPERTY_G_BE = "g_be";
    const Ogre::String Armor::PROPERTY_G_RS = "g_rs";

	Armor::Armor(unsigned int id)
		: Item(id),
		mGRS(0),
		mGBE(0)
	{
        mQueryFlags |= QUERYFLAG_ARMOR;
	}

	Armor::~Armor(void)
	{
	}

	int Armor::getKo()
	{
		return mKo;
	}
	int Armor::getBr()
	{
		return mBr;
	}
	int Armor::getRue()
	{
		return mRue;
	}
	int Armor::getBa()
	{
		return mBa;
	}
	int Armor::getLA()
	{
		return mLA;
	}
	int Armor::getRA()
	{
		return mRA;
	}
	int Armor::getLB()
	{
		return mLB;
	}
	int Armor::getRB()
	{
		return mRB;
	}
	int Armor::getGes()
	{
		return mGes;
	}
	int Armor::getGRS()
	{
		return mGRS;
	}
	int Armor::getGBE()
	{
		return mGBE;
	}
	// und Setter
	void Armor::setKo(int ko)
	{
		mKo = ko;
	}
	void Armor::setBr(int br)
	{
		mBr = br;
	}
	void Armor::setRue(int rue)
	{
		mRue = rue;
	}
	void Armor::setBa(int ba)
	{
		mBa = ba;
	}
	void Armor::setLA(int la)
	{
		mLA = la;
	}
	void Armor::setRA(int ra)
	{
		mRA = ra;
	}
	void Armor::setLB(int lb)
	{
		mLB = lb;
	}
	void Armor::setRB(int rb)
	{
		mRB = rb;
	}
	void Armor::setGes(int ges)
	{
		mGes = ges;
	}
	void Armor::setGRS(int grs)
	{
		mGRS = grs;
	}
	void Armor::setGBE(int gbe)
	{
		mGBE = gbe;
	}

    void Armor::setProperty(const Ogre::String &key, const rl::Property &value)
    {
        if (key == Armor::PROPERTY_G_BE)
        {
            mGBE = value.toInt();
        }
        else if (key == Armor::PROPERTY_G_RS)
        {
            mGRS = value.toInt();
        }
        else
        {
            Item::setProperty(key, value);
        }
    }

    const Property Armor::getProperty(const Ogre::String &key) const
    {
        if (key == Armor::PROPERTY_G_BE)
        {
            return Property(mGBE);
        }
        else if (key == Armor::PROPERTY_G_RS)
        {
            return Property(mGRS);
        }
        else
        {
            return Item::getProperty(key);
        }
    }

    PropertySet* Armor::getAllProperties() const
    {
        PropertySet* ps = Item::getAllProperties();
        ps->setProperty(Armor::PROPERTY_G_BE, Property(mGBE));
        ps->setProperty(Armor::PROPERTY_G_RS, Property(mGRS));

        return ps;
    }
}

