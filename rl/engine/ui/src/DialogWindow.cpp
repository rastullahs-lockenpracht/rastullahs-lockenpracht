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

#include "DialogWindow.h"

#include <CEGUI/widgets/FrameWindow.h>
#include <CEGUI/widgets/ListboxTextItem.h>
#include <boost/bind.hpp>

#include "ConfigurationManager.h"
#include "Creature.h"
#include "DebugWindow.h"
#include "DialogControlState.h"
#include "DialogOption.h"
#include "DialogParagraph.h"
#include "DialogResponse.h"
#include "Exception.h"
#include "GameLoggerWindow.h"
#include "InputManager.h"
#include "UiSubsystem.h"

namespace rl
{

    using namespace Ogre;
    using namespace CEGUI;

    const CEGUI::Colour DialogWindow::COLOR_PLAYER_CHARACTER(0xFFFF7F7F);
    const CEGUI::Colour DialogWindow::COLOR_NON_PLAYER_CHARACTER(0xFFFFFF7F);

    DialogWindow::DialogWindow(DialogControlState* controller)
        : AbstractWindow("dialogwindow.xml", WIT_MOUSE_INPUT, false)
        , mController(controller)
    {
        mImage = getWindow("DialogWindow/Image");
        mName = getWindow("DialogWindow/Name");
        mQuestion = getListbox("DialogWindow/Question");
        mDialogOptions = getListbox("DialogWindow/OptionList");

        mWindow->subscribeEvent(FrameWindow::EventCloseClicked, // Verstecken, falls Close geklickt wird
            boost::bind(&DialogControlState::requestDialogClose, controller)); // TODO: als Abbrechen werten

        mDialogOptions->subscribeEvent(
            Listbox::EventSelectionChanged, boost::bind(&DialogWindow::handleDialogSelectOption, this));

        mDialogOptions->moveToFront();
        mDialogOptions->setClippedByParent(true);
        mDialogOptions->setShowHorzScrollbar(false);
        mDialogOptions->setShowVertScrollbar(false);
        mDialogOptions->setMultiselectEnabled(false);

        // Add 2 ListboxItems, one for the nsc responses,
        // one for the player selections
        mPlayerText = new ListboxTextItem("", 6);
        mPlayerText->setTextColours(COLOR_PLAYER_CHARACTER);
        mQuestion->addItem(mPlayerText);

        mNscText = new ListboxTextItem("", 0);
        mNscText->setTextColours(COLOR_NON_PLAYER_CHARACTER);
        mQuestion->addItem(mNscText);
    }

    void DialogWindow::setImage(const CeGuiString& imageset, const CeGuiString& image)
    {
        mImage->setProperty("Image", "set:" + imageset + " image:" + image);
    }

    void DialogWindow::setName(const CeGuiString& name)
    {
        mName->setText(name);
    }

    void DialogWindow::setDialogEnd()
    {
        setChosenOption("Ende");
        setResponse("");
    }

    void DialogWindow::setAvailableOptions(const DialogResponse::Options& options)
    {
        unsigned int optionCount = 0;
        for (DialogResponse::Options::const_iterator itr = options.begin(); itr != options.end(); ++itr)
        {
            DialogOption* cur = *itr;

            ListboxTextItem* item = NULL;
            CeGuiString text;
            if (cur->getLabel().empty())
            {
                text = cur->getParagraphs(NULL).front()->getText();
            }
            else
            {
                text = cur->getLabel();
            }
            if (cur->isConditional())
            {
                CeGuiString type = cur->getConditionVariableType();
                type.c_str();
                if (type == "talentcheck")
                {
                    text += " [T] ";
                }
                else if (type == "attributecheck")
                {
                    text += " [E] ";
                }
            }

            if (optionCount < mDialogOptions->getItemCount())
            {
                item = static_cast<ListboxTextItem*>(mDialogOptions->getListboxItemFromIndex(optionCount));
                item->setText(text);
                item->setUserData(cur);
            }
            else
            {
                item = new ListboxTextItem(text, 6, NULL, true);
                item->setUserData(cur);
                mDialogOptions->addItem(item);
            }

            // item->setTextFormatting(CEGUI::WordWrapLeftAligned);
            mDialogOptions->handleUpdatedItemData();

            ++optionCount;
        }

        while (optionCount < mDialogOptions->getItemCount())
        {
            mDialogOptions->removeItem(mDialogOptions->getListboxItemFromIndex(optionCount));
        }
    }

    void DialogWindow::setChosenOption(const CeGuiString& option)
    {
        mPlayerText->setText("Held: " + option);
    }

    void DialogWindow::setResponse(const CeGuiString& response)
    {
        mNscText->setText(mName->getText() + ": " + response);
        mQuestion->handleUpdatedItemData();
    }

    bool DialogWindow::handleDialogSelectOption()
    {
        CEGUI::ListboxItem* curr = mDialogOptions->getFirstSelectedItem();
        if (curr)
        {
            return mController->handleDialogSelectOption(static_cast<DialogOption*>(curr->getUserData()));
        }
        else
        {
            return -1;
        }
    }
}
