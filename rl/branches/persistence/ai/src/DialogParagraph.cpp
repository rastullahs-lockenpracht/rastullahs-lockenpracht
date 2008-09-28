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

namespace rl
{

    DialogParagraph::DialogParagraph(
        const CeGuiString& text, const Ogre::String& voicefile)
    : mText(text), mVoiceFile(voicefile)
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

    const Ogre::String& DialogParagraph::getVoiceFile() const
    {
        return mVoiceFile;
    }

    DialogResponse* DialogParagraph::getResponse() const
    {
        return NULL;
    }

    DialogGotoResponse::DialogGotoResponse(rl::DialogResponse *response)
        : DialogParagraph("", ""), mResponse(response)
    {
    }

    DialogResponse* DialogGotoResponse::getResponse() const
    {
        return mResponse;
    }
}

