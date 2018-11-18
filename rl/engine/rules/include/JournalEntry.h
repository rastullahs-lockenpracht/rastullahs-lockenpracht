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

#ifndef __JOURNALENTRY_H__
#define __JOURNALENTRY_H__

#include "RulesPrerequisites.h"
#include "EventObject.h"

namespace rl {

    class QuestBook;

    /**
    * @brief Ein Tagebucheintrag
    * @todo Mit Zeitstempel versehen
    * @todo Mit Quest verknuepfbar machen
    */
    class _RlRulesExport JournalEntry
    {
    public:
        static const Ogre::String PROPERTY_TEXT;
        static const Ogre::String PROPERTY_CAPTION;

        JournalEntry(const CeGuiString caption, const CeGuiString text);
        virtual ~JournalEntry();

        CeGuiString getCaption() const;

        CeGuiString getText() const;

    private:
        CeGuiString mCaption;
        CeGuiString mText;
    };

    class _RlRulesExport JournalEvent : public virtual EventObject 
    {
    public:
        static const unsigned int JOURNAL_ENTRY_ADDED = 1;
        static const unsigned int JOURNAL_ENTRY_DELETED = 2;

        JournalEvent(QuestBook* src, int reason, JournalEntry* entry);
        virtual ~JournalEvent();

        JournalEntry* getJournalEntry() const;
    private:
        JournalEntry* mJournalEntry;
    };

}
#endif
