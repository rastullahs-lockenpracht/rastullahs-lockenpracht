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
#include "stdinc.h" //precompiled header

#include "JournalEntry.h"
#include "QuestBook.h"

namespace rl
{
    const Ogre::String JournalEntry::PROPERTY_TEXT = "text";
    const Ogre::String JournalEntry::PROPERTY_CAPTION = "caption";

    JournalEntry::JournalEntry(const CeGuiString caption, const CeGuiString text)
        : mCaption(caption), mText(text)
    {
    }

    JournalEntry::~JournalEntry()
    {
    }

    CeGuiString JournalEntry::getCaption() const
    {
        return mCaption;
    }

    CeGuiString JournalEntry::getText() const
    {
        return mText;
    }

    JournalEvent::JournalEvent(QuestBook* src, int reason, JournalEntry* entry)
        : EventObject(src, reason), mJournalEntry(entry)
    {
    }

    JournalEvent::~JournalEvent()
    {
    }

    JournalEntry* JournalEvent::getJournalEntry() const
    {
        return mJournalEntry;
    }
}
