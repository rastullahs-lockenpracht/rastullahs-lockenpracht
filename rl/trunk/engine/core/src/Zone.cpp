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
#include "Zone.h"

#include "ActorManager.h"
#include "GameAreaEvent.h"
#include "GameEventManager.h"
#include "ZoneManager.h"


namespace rl 
{

	Zone::Zone()
		: GameAreaListener()
	{
	}

	void Zone::areaLeft(GameAreaEvent *anEvent)
	{
		ZoneManager::getSingleton().areaLeft(this);
	}

    void Zone::areaEntered(GameAreaEvent *anEvent)
	{		
		ZoneManager::getSingleton().areaEntered(this);
	}

	void Zone::addLight(Actor* lo)
	{
		mLights.push_back(lo);
	}

	std::vector<Actor*> Zone::getLights() const
	{
		std::vector<Actor*> rval(mLights);
		return rval;
	}

}
