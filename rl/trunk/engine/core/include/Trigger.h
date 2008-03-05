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
#ifndef __Trigger_H__
#define __Trigger_H__

#include "CorePrerequisites.h"

#include "Properties.h"

namespace rl
{
	class _RlCoreExport Trigger : public PropertyHolder
	{
	public:
        Trigger(const Ogre::String& classname, const Ogre::String &name, bool needsToBeSaved);
        virtual ~Trigger() {}
        /**
         * return true, if the trigger can be deleted after execution. 
         * Using ZoneManager::destroyZone in this function is explicitly allowed 
         * (and intended); the ZoneManager cares for deleted zones, while executing triggers!
        */
		virtual bool activate() = 0;
        /**
         * return true, if the trigger can be deleted after execution. 
         * Using ZoneManager::destroyZone in this function is explicitly allowed 
         * (and intended); the ZoneManager cares for deleted zones, while executing triggers!
        */
		virtual bool deactivate() = 0;
        /**
         * if the trigger should also be removed, if the corresponding zone is destroyd, return true
         * 
        */
        virtual bool deleteIfZoneDestroyed() const {return true;}

        const Ogre::String& getName() const {return mName;}
        const Ogre::String& getClassName() const {return mClassName;}

        bool needsToBeSaved() const {return mNeedsToBeSaved;}

        virtual const Property getProperty(const Ogre::String& key) const;
        virtual void setProperty(const Ogre::String& key, const Property& value);
        virtual PropertyRecord* getAllProperties() const;
    protected:
        Ogre::String mName;
        Ogre::String mClassName;
	private:
        const bool mNeedsToBeSaved;
	};
}

#endif // __Trigger_H__
