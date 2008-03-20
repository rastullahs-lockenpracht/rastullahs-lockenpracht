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

#ifndef __Rl_CoreMessages_H__
#define __Rl_CoreMessages_H__

#include "CorePrerequisites.h"
#include "MessageType.h"

namespace rl
{
    class BaseAnimation;

    enum CoreMessageTypeIds
    {
        // World //
        // Mask
        RLMSG_SCENE_EVENTS =            0x01000100,
        // Events
        RLMSG_SCENE_LOADED =            0x01000101,
        RLMSG_SCENE_CLEARING =          0x01000102,

        // Animations //
        // Mask
        RLMSG_ANIMATION_EVENTS =        0x01000200,
        // Events
        RLMSG_ANIMATION_PAUSED =        0x01000201,
        RLMSG_ANIMATION_UNPAUSED =      0x01000202,
        RLMSG_ANIMATION_FINISHED =      0x01000203,
        RLMSG_ANIMATION_FRAMEREACHED =  0x01000204,

        //Mask
        RLMSG_SAVEGAME_EVENTS     =     0x01000300,
        //Events    
        RLMSG_SAVEGAME_LOADED     =     0x01000301,
        RLMSG_SAVEGAME_SAVED      =     0x01000302,
        RLMSG_SAVEGAME_LOADING    =     0x01000303,
        RLMSG_SAVEGAME_SAVING     =     0x01000304,
        RLMSG_SAVEGAME_GOS_LOADED =     0x01000311       
    };

    /// Message sent right after a scene has been loaded.
    typedef MessageType<RLMSG_SCENE_LOADED> MessageType_SceneLoaded;

    /// Message sent just before a scene will be cleared.
    typedef MessageType<RLMSG_SCENE_CLEARING> MessageType_SceneClearing;

    /// Message sent when an animation has been paused.
    typedef MessageType<RLMSG_ANIMATION_PAUSED, BaseAnimation*> MessageType_AnimationPaused;

    /// Message sent when an animation has been unpaused.
    typedef MessageType<RLMSG_ANIMATION_UNPAUSED, BaseAnimation*> MessageType_AnimationUnpaused;

    /// Message sent when an animation is finished.
    typedef MessageType<RLMSG_ANIMATION_FINISHED, BaseAnimation*> MessageType_AnimationFinished;

    /// Message sent when an animation has been paused.
    /// Real param is the frame time reached
    typedef MessageType<RLMSG_ANIMATION_FRAMEREACHED, BaseAnimation*, Ogre::Real>
        MessageType_AnimationFrameReached;

    typedef MessageType<RLMSG_SAVEGAME_LOADED> MessageType_SaveGameLoaded;
    
    typedef MessageType<RLMSG_SAVEGAME_SAVED> MessageType_SaveGameSaved;
    
    typedef MessageType<RLMSG_SAVEGAME_LOADING> MessageType_SaveGameLoading;
    
    typedef MessageType<RLMSG_SAVEGAME_SAVING> MessageType_SaveGameSaving;
    
}
#endif
