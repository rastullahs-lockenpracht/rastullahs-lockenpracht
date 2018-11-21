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

#ifndef __XdimlLoader_H__
#define __XdimlLoader_H__

#include "XmlProcessor.h"

namespace rl
{
    class Creature;
    class Talent;
    class Kampftechnik;

    class XdimlLoader : public Ogre::ScriptLoader, private XmlProcessor
    {
    public:
        XdimlLoader();

        virtual void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);
        virtual const Ogre::StringVector& getScriptPatterns() const;
        virtual Ogre::Real getLoadingOrder() const;

    private:
        int getEBeFromString(const Ogre::String& eBeString);

        void initializeTalente(const tinyxml2::XMLElement* rootTalente);
        Talent* processTalent(int gruppe, const tinyxml2::XMLElement* talentXml);

        void initializeCreatures(const tinyxml2::XMLElement* rootCreatures);
        Creature* processCreature(const tinyxml2::XMLElement* talentXml);

        void initializeKampftechniken(const tinyxml2::XMLElement* rootKampftechniken);
        Kampftechnik* processKampftechnik(const tinyxml2::XMLElement* kampftechnikXml);

        Ogre::StringVector mScriptPatterns;
    };
}

#endif
