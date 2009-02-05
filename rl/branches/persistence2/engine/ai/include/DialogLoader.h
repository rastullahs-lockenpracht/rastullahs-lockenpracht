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
#ifndef __DialogLoader_H__
#define __DialogLoader_H__

#include "AiPrerequisites.h"

namespace rl 
{
	class Creature;
    class Dialog;
    class DialogLoaderImpl;

	class DialogLoader : public Ogre::ScriptLoader
	{
	public:
		DialogLoader();
		~DialogLoader();
	
		virtual const Ogre::StringVector& getScriptPatterns() const;
        virtual void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);
		virtual Ogre::Real getLoadingOrder() const;
		
		Dialog* createDialog(const Ogre::String& name, const std::vector<Creature*>& pcs, const std::vector<Creature*>& npcs) const;

	private: 
        Ogre::StringVector mScriptPatterns;
        DialogLoaderImpl* mLoaderImplementation;
    };
}

#endif //__DialogLoader_H__
