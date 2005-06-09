/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#ifndef __GameEventManager_H__
#define __GameEventManager_H__

#include <map>
#include <OgreSingleton.h>

#include "GameTask.h"
#include "CorePrerequisites.h"

namespace rl {

class Actor;
/** GameEventManager
 *  
 *  @see GameAreaListener, GameAreaEventSource, GameAreaEvent, GameAreaTypes
 */
class _RlCoreExport GameEventManager
  : public GameTask,
    protected Ogre::Singleton<GameEventManager>
{
public:
    /** Default Constructor */
    GameEventManager( );
    /** Default Deconstructor */
    virtual ~GameEventManager();

    /** Wird vom Gameloop aufgerufen, wenn nicht pausiert
     * @param elapsedTime Die vergangene Zeit
     */
    virtual void run( Ogre::Real elapsedTime );

    /// Singleton
    static GameEventManager & getSingleton(void);
    /// Singleton
    static GameEventManager * getSingletonPtr(void);
private:
};

}

#endif
