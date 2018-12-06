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

#include "UiPrerequisites.h"
#include <vector>

#include "AbstractWindow.h"
#include "QuestListener.h"

namespace rl
{

    class _RlUiExport InfoPopup : public AbstractWindow, public QuestListener
    {
    public:
        InfoPopup();
        ~InfoPopup();

        void showError();
        void showQuestBookChange();

        void windowHid();

        void setVisible(bool visible, bool destroyAfterHide = false);

        void questStateChanged(QuestEvent* anEvent);
        void questPartsDoneChanged(QuestEvent* anEvent);
        void questKnownChanged(QuestEvent* anEvent);
        void questSubquestAdded(QuestEvent* anEvent);

        void journalEntryAdded(JournalEvent* anEvent);
        void journalEntryDeleted(JournalEvent* anEvent);

    private:
        static const int ICON_SPACING = 5;

        void showMessageIcon(CEGUI::Window* image);

        std::vector<CEGUI::Window*> mActiveIcons;
        CEGUI::Window* mErrorIcon;
        CEGUI::Window* mQuestIcon;
    };
}
