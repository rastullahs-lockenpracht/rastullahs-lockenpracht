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

#include "GameAreaListener.h"

#include "GameEventManager.h"
#include "CoreSubsystem.h"
#include "Exception.h"

namespace rl {

    GameAreaListener::~GameAreaListener()
    {
        GameEventManager::getSingleton().removeAreaListener( this );
    }


    bool GameAreaListener::eventRaised( GameAreaEvent* anEvent )
    {
        try
        {
            switch( anEvent->getReason() )
            {
            case GameAreaEvent::AREA_ENTERED:
                this->areaEntered( anEvent );
                break;
            case GameAreaEvent::AREA_LEFT:
                this->areaLeft( anEvent );
                break;
            }
        }
        catch( ScriptInvocationFailedException& sife )
        {
            Logger::getSingleton().log(Logger::CORE, Logger::LL_ERROR, sife.toString() );
        }


        // consumed or not ;)
        return false;
    }

}

