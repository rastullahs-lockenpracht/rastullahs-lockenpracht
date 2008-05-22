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
#include "stdinc.h"

#include "Dialog.h"
#include "DialogResponse.h"

namespace rl
{
	const Ogre::String Dialog::PROP_EXIT_REQUESTED = "exit_requested";

    Dialog::Dialog(const std::vector<Creature*>& npc, const std::vector<Creature*>& pc)
    {
        mNonPlayerCharacters = npc;
        mPlayerCharacters = pc;
    }

    Dialog::~Dialog()
    {
    }

    DialogResponse* Dialog::getDialogStart() const
    {
        return mDialogStart;
    }

    void Dialog::setStartResponse(DialogResponse* start)
    {
        mDialogStart = start;
    }

    void Dialog::initialize() ///@todo hand over NPCs and party
    {
		mExitRequested = false;
    }

    void Dialog::addVariable(DialogVariable* variable)
    {
        //mVariables[vkey] = variable;
    }

    Creature* Dialog::getNpc(int id) const
    {
        return mNonPlayerCharacters[id];
    }

    Creature* Dialog::getPc(int id) const
    {
        return mPlayerCharacters[id];
    }

    const Property Dialog::getProperty(const CeGuiString& key) const
    {
        ///@todo dialog's state
		if (key == Dialog::PROP_EXIT_REQUESTED)
		{
			return mExitRequested;
		}

        return mPropertyVariables.getProperty(key);
    }

    void Dialog::setProperty(const CeGuiString& key, const Property& value)
    {
		if (key == Dialog::PROP_EXIT_REQUESTED)
		{
			mExitRequested = value;
		}
        ///@todo dialog's state
        mPropertyVariables.setProperty(key, value);
    }

    PropertyKeys Dialog::getAllPropertyKeys() const
    {
        PropertyKeys keys;
        keys.insert(Dialog::PROP_EXIT_REQUESTED);
        ///@todo dialog's state

        PropertyKeys varKeys = mPropertyVariables.getAllPropertyKeys();
        keys.insert(varKeys.begin(), varKeys.end());
        return keys;
    }

	bool Dialog::isExitRequested() const
	{
		return mExitRequested;
	}

    CeGuiString Dialog::getVariableValue(const Ogre::String& key) const
    {
        return getProperty(key).getAsString();
    }

    std::vector<Creature*> Dialog::getNonPlayerCharacters() const
    {
        return mNonPlayerCharacters;
    }

    std::vector<Creature*> Dialog::getPlayerCharacters() const
    {
        return mPlayerCharacters;
    }

}
