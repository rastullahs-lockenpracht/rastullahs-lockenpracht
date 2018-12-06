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

#include "DialogParagraph.h"

#include "Dialog.h"
#include "DialogElement.h"

using namespace std;

namespace rl
{

    DialogParagraph::DialogParagraph(const CeGuiString& text, const CeGuiString& person, const Ogre::String& voicefile)
        : mText(text)
        , mPerson(person)
        , mVoiceFile(voicefile)
        , mParent(NULL)
    {
        // for debug information
        mText.c_str();
    }

    DialogParagraph::~DialogParagraph(void)
    {
    }

    const CeGuiString& DialogParagraph::getText() const
    {
        return mText;
    }

    const CeGuiString& DialogParagraph::getPerson() const
    {
        if (mPerson.empty())
        {
            return mParent->getPerson();
        }
        return mPerson;
    }

    const Ogre::String& DialogParagraph::getVoiceFile() const
    {
        return mVoiceFile;
    }

    DialogResponse* DialogParagraph::getResponse() const
    {
        return NULL;
    }

    DialogElement* DialogParagraph::getParent() const
    {
        return mParent;
    }

    void DialogParagraph::_setParent(DialogElement* parent)
    {
        mParent = parent;
    }

    Creature* DialogParagraph::getSpeaker(Dialog* dialog) const
    {
        return dialog->getParticipant(getPerson());
    }

    CreatureList DialogParagraph::getListeners(Dialog* dialog) const
    {
        Creature* speaker = getSpeaker(dialog);

        CreatureList listeners;

        CreatureList participants = dialog->getParticipants();
        for (CreatureList::iterator it = participants.begin(), end = participants.end(); it != end; ++it)
        {
            if (*it != speaker)
            {
                listeners.push_back(*it);
            }
        }

        return listeners;
    }

    DialogGotoResponse::DialogGotoResponse(rl::DialogResponse* response)
        : DialogParagraph("", "", "")
        , mResponse(response)
    {
    }

    DialogResponse* DialogGotoResponse::getResponse() const
    {
        return mResponse;
    }
}
