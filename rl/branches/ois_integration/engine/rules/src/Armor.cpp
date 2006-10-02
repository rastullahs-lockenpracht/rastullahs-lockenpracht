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
	Armor::Armor(const CeGuiString weaponName, const CeGuiString description)
		: Item(weaponName, description)
	{

	}

	Armor::~Armor(void)
	{
	}

	Armor* Armor::clone()
	{
		Armor* a = new Armor(mName,mDescription);
		a->setImageName(mImageName);
		a->setMeshName(mMeshName);
		a->setItemType(mItemType);
		a->setSize(mSize.first,mSize.second);
		a->setContainer(mIsContainer,mCapacity);
		a->setKo(mKo);
		a->setBr(mBr);
		a->setRue(mRue);
		a->setBa(mBa);
		a->setLA(mLA);
		a->setRA(mRA);
		a->setLB(mLB);
		a->setRB(mRB);
		a->setGes(mGes);
		a->setGRS(mGRS);
		a->setGBE(mGBE);
		// Umrechnung Stein->Unzen = Mal 40
		a->setWeight(mWeight);
		a->setPrice(mPrice);
		return a;
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

