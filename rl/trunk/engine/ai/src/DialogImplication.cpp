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
#include "stdinc.h"

#include "DialogImplication.h"

#include "Dialog.h"
#include "Property.h"

using namespace Ogre;

namespace rl
{

    DialogImplication::DialogImplication()
    {
    }

    DialogImplication::~DialogImplication()
    {
    }

    DialogVariableAssignment::DialogVariableAssignment(const Ogre::String& variableName, const CeGuiString& value)
        : mVariableName(variableName), mValue(value)
    {
    }

    DialogVariableAssignment::~DialogVariableAssignment()
    {
    }

    void DialogVariableAssignment::apply(Dialog* dialog)
    {
        Property prop = dialog->getProperty(mVariableName);
        prop.getFromString(mValue);
        dialog->setProperty(mVariableName, prop);
    }

	void DialogExit::apply(rl::Dialog *dialog)
	{
		dialog->setProperty(Dialog::PROP_EXIT_REQUESTED, true);
	}

}
