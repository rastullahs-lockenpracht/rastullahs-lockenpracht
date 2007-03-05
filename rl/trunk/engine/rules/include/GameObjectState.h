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

#ifndef __GAMEOBJECTSTATE_H__
#define __GAMEOBJECTSTATE_H__

namespace rl {

    /// Basic states, a GameObject can have.
    /// Values are chosen so, that they are maskable.
    enum GameObjectState
    {
        GOS_UNDEFINED     =  0, ///!< Nothing known about such a GO.
        GOS_UNLOADED      =  1, ///!< GO is known, but its actual properties are not loaded yet.
        GOS_LOADED        =  2, ///!< GO is loaded, unused.
        GOS_IN_POSSESSION =  4, ///!< GO is owned by another GO. (e.g. inside a chest)
        GOS_IN_SCENE      =  8, ///!< GO is placed openly in the scene.
        GOS_HELD          = 16, ///!< GO is held openly by another GO. (e.g. apple in NPC's hand)
        GOS_READY         = 32  ///!< GO is ready for usage by another GO. (sword in NPC's hand)
    };
}

#endif // GAMEOBJECTSTATE
