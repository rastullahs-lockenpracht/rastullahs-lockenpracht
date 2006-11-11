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

#include "DsaManager.h"
#include "Armor.h"

using namespace std;

namespace rl
{
    const Ogre::String Armor::CLASS_NAME = "Armor";

	Armor::Armor(unsigned int id)
		: Item(id)
	{

	}

	Armor::~Armor(void)
	{
	}

	const CeGuiString Armor::getDescription() const
	{
		return mName + "\r\n\r\n" + 
			getFormattedText(mDescription);	
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

}

