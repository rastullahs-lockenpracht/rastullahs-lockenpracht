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

#ifndef __DialogParagraph_H__
#define __DialogParagraph_H__

#include "AiPrerequisites.h"
#include "Creature.h"

namespace rl
{
    class Dialog;
    class DialogElement;
    class DialogResponse;

    class _RlAiExport DialogParagraph
    {
    public:
        DialogParagraph(const CeGuiString& text, const CeGuiString& person = "", const Ogre::String& voicefile = "");
        virtual ~DialogParagraph();

        const CeGuiString& getText() const;
        const CeGuiString& getPerson() const;
        const Ogre::String& getVoiceFile() const;
        DialogElement* getParent() const;
        void _setParent(DialogElement* parent);
        virtual DialogResponse* getResponse() const;
        Creature* getSpeaker(Dialog* dialog) const;
        CreatureList getListeners(Dialog* dialog) const;

    private:
        CeGuiString mText;
        CeGuiString mPerson;
        Ogre::String mVoiceFile;
        DialogElement* mParent;
    };

    class _RlAiExport DialogGotoResponse : public DialogParagraph
    {
    public:
        DialogGotoResponse(DialogResponse* response);
        virtual DialogResponse* getResponse() const;
    private:
        DialogResponse* mResponse;
    };
}

#endif // __DialogParagraph_H__
