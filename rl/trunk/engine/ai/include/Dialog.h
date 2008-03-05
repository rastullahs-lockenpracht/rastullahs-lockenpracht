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
#ifndef __Dialog_H__
#define __Dialog_H__

#include "AiPrerequisites.h"

#include "Properties.h"

namespace rl
{
    class DialogOption;
    class DialogResponse;
    class DialogVariable;
    class Creature;

    class _RlAiExport Dialog : public PropertyHolder
    {
    public:
		static const Ogre::String PROP_EXIT_REQUESTED;

        Dialog(const std::vector<Creature*>& pc, const std::vector<Creature*>& npc);
        ~Dialog();

        DialogResponse* getDialogStart() const;
        void setStartResponse(DialogResponse* start);
        void addVariable(DialogVariable* variable);
        void initialize();
        Creature* getNpc(int id) const;
        Creature* getPc(int id) const;
        std::vector<Creature*> getNonPlayerCharacters() const;
        std::vector<Creature*> getPlayerCharacters() const;
		bool isExitRequested() const;
        CeGuiString getVariableValue(const Ogre::String& variableName) const;

        virtual const Property getProperty(const Ogre::String& key) const;
        virtual void setProperty(const Ogre::String& key, const Property& value);
        virtual PropertyRecord* getAllProperties() const;

    private:
        DialogResponse* mDialogStart;
        std::vector<Creature*> mNonPlayerCharacters;
        std::vector<Creature*> mPlayerCharacters;
        std::map<Ogre::String, DialogVariable*> mVariables;
        PropertyRecord mPropertyVariables;
		bool mExitRequested;
    };

}

#endif // __Dialog_H__
