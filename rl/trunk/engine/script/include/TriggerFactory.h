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
#ifndef __TriggerFactory_H__
#define __TriggerFactory_H__

#include "SaveGameManager.h"
#include "ScriptPrerequisites.h"

namespace rl 
{
	class Trigger;

    class _RlScriptExport TriggerFactory :
        public SaveGameData // in order to save and restore trigger
	{
	public:
        TriggerFactory();
		virtual ~TriggerFactory();
        virtual Trigger* createTrigger(const Ogre::String& classname, const Ogre::String& triggername) = 0;


        /// Override from SaveGameData
        /// Manages saving and loading from the SaveGameFile

        virtual CeGuiString getXmlNodeIdentifier() const;
        virtual void writeData(SaveGameFileWriter* writer);
        virtual void readData(SaveGameFileReader* reader);
        virtual int getPriority() const;  // triggers must be loaded AFTER zones
	};
}

#endif // __TriggerFactory_H__
