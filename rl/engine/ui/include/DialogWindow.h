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

#ifndef __DialogWindow_H__
#define __DialogWindow_H__

#include "UiPrerequisites.h"

#include <map>
#include <vector>

#include "AbstractWindow.h"
#include "DialogResponse.h"

namespace rl
{

    class DialogControlState;

    class _RlUiExport DialogWindow : public AbstractWindow
    {
    public:
        DialogWindow(DialogControlState* controller);

        void setName(const CeGuiString& name);
        void setImage(const CeGuiString& imageset, const CeGuiString& image);
        void setAvailableOptions(const DialogResponse::Options& options);
        void setChosenOption(const CeGuiString& option);
        void setResponse(const CeGuiString& response);
        void setDialogEnd();
        int getSelectedOptionIndex() const;

    private:
        bool handleDialogSelectOption();

        static const CEGUI::Colour COLOR_PLAYER_CHARACTER;
        static const CEGUI::Colour COLOR_NON_PLAYER_CHARACTER;

        DialogControlState* mController;

        CEGUI::Listbox* mDialogOptions;
        CEGUI::Window* mImage;
        CEGUI::Listbox* mQuestion;
        CEGUI::Window* mName;
        CEGUI::ListboxTextItem* mPlayerText;
        CEGUI::ListboxTextItem* mNscText;
    };
}
#endif
