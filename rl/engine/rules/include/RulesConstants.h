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

#ifndef __RULESCONSTANTS_H__
#define __RULESCONSTANTS_H__

namespace rl
{

    /// Basic states, a GameObject can have.
    /// Values are chosen so, that they are maskable.
    enum GameObjectState
    {
        GOS_UNDEFINED = 0, ///!< Nothing known about such a GO.
        GOS_UNLOADED = 1 << 0, ///!< GO is known, but its actual properties are not loaded yet.
        GOS_LOADED = 1 << 1, ///!< GO is loaded, unused.
        GOS_IN_POSSESSION = 1 << 2, ///!< GO is owned by another GO. (e.g. inside a chest)
        GOS_IN_SCENE = 1 << 3, ///!< GO is placed openly in the scene.
        GOS_HELD = 1 << 4, ///!< GO is held openly by another GO. (e.g. apple in NPC's hand)
        GOS_READY = 1 << 5 ///!< GO is ready for usage by another GO. (sword in NPC's hand)
    };

    static const unsigned long QUERYFLAG_GAMEOBJECT = 1 << 1;
    static const unsigned long QUERYFLAG_CREATURE = 1 << 2;
    static const unsigned long QUERYFLAG_ITEM = 1 << 3;
    static const unsigned long QUERYFLAG_CONTAINER = 1 << 4;
    static const unsigned long QUERYFLAG_WEAPON = 1 << 5;
    static const unsigned long QUERYFLAG_ARMOR = 1 << 6;
    static const unsigned long QUERYFLAG_PERSON = 1 << 7;

    static const unsigned long QUERYFLAG_PLAYER = 1 << 8;

    enum SlotType
    {
        SLOT_BONE,
        SLOT_SUBMESH,
        SLOT_MATERIAL,
        SLOT_DEFAULT ///!< No visual attachment, but only logical one.
    };

    /// Event type that are distinguished in the game event log
    enum GameEventType
    {
        GET_DEFAULT, ///!< If nothing else fits..
        GET_COMBAT, ///!< Combat events ("Alrike hits Wolf for 10TP")
        GET_DIALOG, ///!< Dialog events ("Charisma roll succeded.")
        GET_QUEST ///!< Quest related events ("Quest succeded: Kill Wolf.")
    };
}

#endif // __RULESCONSTANTS_H__
