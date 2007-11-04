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
#ifndef __Dialog_H__
#define __Dialog_H__

#include "AiPrerequisites.h"

#include "Properties.h"

namespace rl
{
    class DialogOption;
    class DialogResponse;
    class GameObject;

    class _RlAiExport Dialog : public PropertyHolder
    {
    public:
		static const Ogre::String PROP_EXIT_REQUESTED;

        Dialog(GameObject* pc, GameObject* npc);
        ~Dialog();

        DialogResponse* getDialogStart() const;
        void setStartResponse(DialogResponse* start);
        void initialize();
        GameObject* getNpc(int id) const;
        GameObject* getPc(int id) const;
		bool isExitRequested() const;

        virtual const Property getProperty(const Ogre::String& key) const;
        virtual void setProperty(const Ogre::String& key, const Property& value);
        virtual PropertyRecord* getAllProperties() const;

    private:
        DialogResponse* mDialogStart;
        std::vector<GameObject*> mNonPlayerCharacters;
        std::vector<GameObject*> mPlayerCharacters;
        PropertyRecord mPropertyVariables;
		bool mExitRequested;
    };

}

#endif // __Dialog_H__
