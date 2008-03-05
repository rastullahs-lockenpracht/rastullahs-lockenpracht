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

#include "ZauberStateSet.h"

namespace rl
{

	ZauberStateSet::ZauberStateSet() : StateSet()
	{
		mSe = false;
		mLehrmeisterZeiteinheiten = 0;
		mSelbststudiumZeiteinheiten = 0;
	}

	ZauberStateSet::~ZauberStateSet()
	{
	}

	bool ZauberStateSet::getSe()
	{
		return mSe;
	}

	void ZauberStateSet::setSe(bool newSe)
	{
		mSe = newSe;
	}

	int ZauberStateSet::getLehrmeisterZeiteinheiten()
	{
		return mLehrmeisterZeiteinheiten;
	}

	void ZauberStateSet::setLehrmeisterZeiteinheiten( int newLehrmeisterZeiteinheiten )
	{
		mLehrmeisterZeiteinheiten = newLehrmeisterZeiteinheiten;
	}

	int ZauberStateSet::getSelbststudiumZeiteinheiten()
	{
		return mSelbststudiumZeiteinheiten;
	}

	void ZauberStateSet::setSelbststudiumZeiteinheiten( int newSelbststudiumZeiteinheiten )
	{
		mSelbststudiumZeiteinheiten = newSelbststudiumZeiteinheiten;
	}
}
