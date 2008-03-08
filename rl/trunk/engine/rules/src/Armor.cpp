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
#include "stdinc.h" //precompiled header

#include "Armor.h"

#include "Creature.h"
#include "DsaManager.h"
#include "Effect.h"
#include "EffectFactory.h"

using namespace std;

namespace rl
{
    const Ogre::String Armor::CLASS_NAME = "Armor";

    const Ogre::String Armor::PROPERTY_G_BE = "g_be";
    const Ogre::String Armor::PROPERTY_G_RS = "g_rs";

	Armor::Armor(unsigned int id)
		: Item(id),
		mGRS(0),
		mGBE(0),
        mRsEffect(NULL)
	{
        mQueryFlags |= QUERYFLAG_ARMOR;
		mItemType = ITEMTYPE_ARMOR;
	}

	Armor::~Armor(void)
	{
	}

	int Armor::getKo() const
	{
		return mKo;
	}
	int Armor::getBr() const
	{
		return mBr;
	}
	int Armor::getRue() const
	{
		return mRue;
	}
	int Armor::getBa() const
	{
		return mBa;
	}
	int Armor::getLA() const
	{
		return mLA;
	}
	int Armor::getRA() const
	{
		return mRA;
	}
	int Armor::getLB() const
	{
		return mLB;
	}
	int Armor::getRB() const
	{
		return mRB;
	}
	int Armor::getGes() const
	{
		return mGes;
	}
	int Armor::getGRS() const
	{
		return mGRS;
	}
	int Armor::getGBE() const
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

    void Armor::setProperty(const CeGuiString &key, const Property &value)
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

    void Armor::onStateChange(GameObjectState oldState, GameObjectState newState)
    {
        if (oldState != newState)
        {
            if (newState == GOS_READY)
            {
                if (getOwner())
                {
                    mRsEffect = EffectFactoryManager::getSingleton().createEffect(
                        "Ruestung");
                    if(mRsEffect == NULL)
                        return;
                    mRsEffect->setProperty(Armor::PROPERTY_G_BE, Property(mGBE));
                    mRsEffect->setProperty(Armor::PROPERTY_G_RS, Property(mGRS));
                    getOwner()->addEffect(mRsEffect);
                }
            }
            else if (oldState == GOS_READY)
            {
                if (getOwner())
                {
                    if(mRsEffect == NULL)
                        return;
                    getOwner()->removeEffect(mRsEffect);
                    delete mRsEffect;
                }
            }
        }
    }

    const Property Armor::getProperty(const CeGuiString &key) const
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

    PropertyKeys Armor::getAllPropertyKeys() const
    {
        PropertyKeys keys(Item::getAllPropertyKeys());
        keys.insert(Armor::PROPERTY_G_BE);
        keys.insert(Armor::PROPERTY_G_RS);

        return keys;
    }
}

