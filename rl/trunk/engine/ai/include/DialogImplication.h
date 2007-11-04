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

#ifndef __DialogImplication_H__
#define __DialogImplication_H__

#include "AiPrerequisites.h"

namespace rl
{
    class Dialog;

    class _RlAiExport DialogImplication
    {
    public:
        DialogImplication();
        virtual ~DialogImplication();
        
        virtual void apply(Dialog* dialog) = 0;
    };

    class DialogVariableAssignment : public DialogImplication
    {
    public:
        DialogVariableAssignment(const Ogre::String& variableName, const CeGuiString& value);
        virtual ~DialogVariableAssignment();

        virtual void apply(Dialog* dialog);

    private:
        Ogre::String mVariableName; 
        CeGuiString mValue;
    };

	class DialogExit : public DialogImplication
	{
	public:
		virtual void apply(Dialog* dialog);
	};
}

#endif //__DialogImplication_H__
