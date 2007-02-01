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

#ifndef __XdimlLoader_H__
#define __XdimlLoader_H__

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include <OgreScriptLoader.h>

namespace rl {

	class Talent;
	class Person;
	class Kampftechnik;

    class XdimlLoader : public Ogre::ScriptLoader
	{
    public:
        XdimlLoader();

        virtual void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);
        virtual const Ogre::StringVector& getScriptPatterns() const;
		virtual Ogre::Real getLoadingOrder() const;

    private:
		static int getEBeFromString(const Ogre::String& eBeString);

		static void initializeTalente(XERCES_CPP_NAMESPACE::DOMElement* rootTalente);
		static Talent* processTalent(int gruppe, XERCES_CPP_NAMESPACE::DOMElement* talentXml);

		static void initializePersonen(XERCES_CPP_NAMESPACE::DOMElement* rootPersonen);
		static Person* processPerson(XERCES_CPP_NAMESPACE::DOMElement* talentXml);
			
		static void initializeKampftechniken(XERCES_CPP_NAMESPACE::DOMElement* rootKampftechniken);
		static Kampftechnik* processKampftechnik(XERCES_CPP_NAMESPACE::DOMElement* kampftechnikXml);

        Ogre::StringVector mScriptPatterns;
    };
}

#endif
