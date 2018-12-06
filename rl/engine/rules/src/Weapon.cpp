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

#include "DsaManager.h"
#include "Weapon.h"

using namespace std;

namespace rl
{
    const Ogre::Real Weapon::DK_NAHKAMPF_MIN_DISTANCE = 0.5f;
    const Ogre::Real Weapon::DK_STABWAFFEN_MIN_DISTANCE = 1.5f;
    const Ogre::Real Weapon::DK_PIKE_MIN_DISTANCE = 3.0f;
    const Ogre::Real Weapon::DK_PIKE_MAX_DISTANCE = 4.0f;

    const Ogre::String Weapon::CLASS_NAME = "Weapon";
    const Ogre::String Weapon::PROPERTY_TP = "TP";
    const Ogre::String Weapon::PROPERTY_TP_KK = "TPKK";
    const Ogre::String Weapon::PROPERTY_TP_BF = "BF";
    const Ogre::String Weapon::PROPERTY_TP_INI = "INI";
    const Ogre::String Weapon::PROPERTY_TP_WM = "WM";
    const Ogre::String Weapon::PROPERTY_TP_DK = "DK";
    const Ogre::String Weapon::PROPERTY_TP_KAMPFTECHNIK = "kampftechnik";
    const Ogre::String Weapon::PROPERTY_NATURAL = "natural";
    const Ogre::String Weapon::PROPERTY_AVOID_ARMOR = "avoidarmor";

    Weapon::Weapon(unsigned int id)
        : Item(id)
        , mTp(0, 0, 0)
        , mTpKk(0, 0)
        , mBf(0)
        , mIni(0)
        , mWm(0, 0)
        , mDk(DK_H)
        , mKampftechnik("")
        , mNatural(false)
        , mAvoidArmor(false)
    {
        mQueryFlags |= QUERYFLAG_WEAPON;
        mItemType = ITEMTYPE_WEAPON;
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

    Ogre::Real Weapon::getMinimumDistance() const
    {
        if (mDk & DK_H)
        {
            return 0.0f;
        }
        else if (mDk & DK_N)
        {
            return DK_NAHKAMPF_MIN_DISTANCE;
        }
        else if (mDk & DK_S)
        {
            return DK_STABWAFFEN_MIN_DISTANCE;
        }
        else
        {
            return DK_PIKE_MIN_DISTANCE;
        }
    }

    Ogre::Real Weapon::getMaximumDistance() const
    {
        if (mDk & DK_P)
        {
            return DK_PIKE_MAX_DISTANCE;
        }
        else if (mDk & DK_S)
        {
            return DK_PIKE_MIN_DISTANCE;
        }
        else if (mDk & DK_N)
        {
            return DK_STABWAFFEN_MIN_DISTANCE;
        }
        else
        {
            return DK_NAHKAMPF_MIN_DISTANCE;
        }
    }

    void Weapon::setKampftechnik(const CeGuiString& newKampftechnik)
    {
        mKampftechnik = newKampftechnik;
    }

    const CeGuiString& Weapon::getKampftechnik() const
    {
        return mKampftechnik;
    }

    bool Weapon::isNatural() const
    {
        return mNatural;
    }

    void Weapon::setNatural(bool natural)
    {
        mNatural = natural;
    }

    bool Weapon::isAvoidingArmor() const
    {
        return mAvoidArmor;
    }

    void Weapon::setAvoidArmor(bool avoid)
    {
        mAvoidArmor = avoid;
    }

    void Weapon::setProperty(const CeGuiString& key, const Property& value)
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
            Ogre::String tmp = value.toString().c_str();
            mDk = 0;
            for (size_t i = 0; i < tmp.size(); ++i)
            {
                if (tmp[i] == 'H' || tmp[i] == 'h')
                {
                    mDk |= DK_H;
                }
                else if (tmp[i] == 'N' || tmp[i] == 'n')
                {
                    mDk |= DK_N;
                }
                else if (tmp[i] == 'S' || tmp[i] == 's')
                {
                    mDk |= DK_S;
                }
                else if (tmp[i] == 'P' || tmp[i] == 'p')
                {
                    mDk |= DK_P;
                }
            }
        }
        else if (key == Weapon::PROPERTY_TP_KAMPFTECHNIK)
        {
            mKampftechnik = value.toString();
        }
        else if (key == Weapon::PROPERTY_NATURAL)
        {
            mNatural = value.toBool();
        }
        else if (key == Weapon::PROPERTY_AVOID_ARMOR)
        {
            mAvoidArmor = value.toBool();
        }
        else
        {
            Item::setProperty(key, value);
        }
    }

    const Property Weapon::getProperty(const CeGuiString& key) const
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
            CeGuiString dk;
            if (mDk & DK_H)
            {
                dk += "H";
            }
            else if (mDk & DK_N)
            {
                dk += "N";
            }
            else if (mDk & DK_S)
            {
                dk += "S";
            }
            else if (mDk & DK_P)
            {
                dk += "P";
            }
            return Property(dk);
        }
        else if (key == Weapon::PROPERTY_TP_KAMPFTECHNIK)
        {
            return Property(mKampftechnik);
        }
        else if (key == Weapon::PROPERTY_NATURAL)
        {
            return Property(mNatural);
        }
        else if (key == Weapon::PROPERTY_AVOID_ARMOR)
        {
            return Property(mAvoidArmor);
        }
        else
        {
            return Item::getProperty(key);
        }
    }

    PropertyKeys Weapon::getAllPropertyKeys() const
    {
        PropertyKeys keys(Item::getAllPropertyKeys());
        keys.insert(Weapon::PROPERTY_TP);
        keys.insert(Weapon::PROPERTY_TP_KK);
        keys.insert(Weapon::PROPERTY_TP_BF);
        keys.insert(Weapon::PROPERTY_TP_INI);
        keys.insert(Weapon::PROPERTY_TP_WM);
        keys.insert(Weapon::PROPERTY_TP_DK);
        keys.insert(Weapon::PROPERTY_TP_KAMPFTECHNIK);
        keys.insert(Weapon::PROPERTY_NATURAL);
        keys.insert(Weapon::PROPERTY_AVOID_ARMOR);
        return keys;
    }
}
