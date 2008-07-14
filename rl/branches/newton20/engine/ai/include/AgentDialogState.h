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

#ifndef __RlAI_AgentDialogState_H__
#define __RlAI_AgentDialogState_H__

#include "AgentState.h"

namespace rl
{
    class Dialog;

    class _RlAiExport AgentDialogState :
        public AgentState
    {
    public:
        AgentDialogState(Agent* agent);
        ~AgentDialogState();

        virtual void update(const Ogre::Real elapsedTime);
        void setDialogPartner(Agent* partner);
        void setDialog(Dialog* dialog);

    private:
        Agent* mPartner;
        Dialog* mDialog;
        bool mTalking;
    };

}

#endif // __RlAI_AgentDialogState_H__
