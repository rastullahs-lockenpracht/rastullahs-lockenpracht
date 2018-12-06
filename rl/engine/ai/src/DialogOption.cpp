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

#include "Dialog.h"
#include "DialogCondition.h"
#include "DialogOption.h"

namespace rl
{

    DialogOption::DialogOption(const CeGuiString& id, const CeGuiString& person, bool autoSelected)
        : DialogElement(id, person)
        , mPrecondition(NULL)
        , mResponse(NULL)
        , mLabel("")
        , mAutoSelected(autoSelected)
    {
    }

    DialogOption::~DialogOption()
    {
    }

    void DialogOption::setResponse(DialogResponse* response)
    {
        mResponse = response;
    }

    DialogResponse* DialogOption::getResponse() const
    {
        return mResponse;
    }

    void DialogOption::setPrecondition(DialogCondition* precondition)
    {
        mPrecondition = precondition;
    }

    const CeGuiString& DialogOption::getConditionVariableType()
    {
        RlAssert(mPrecondition,
            CeGuiString("No precondition found for option with id: " + getId()
                + "\nA precondition must be set to get its variable type")
                .c_str());
        return mPrecondition->getVariableType();
    }

    bool DialogOption::isConditional()
    {
        return (mPrecondition != NULL);
    }

    void DialogOption::setAutoSelected(bool autoSelected)
    {
        mAutoSelected = autoSelected;
    }

    bool DialogOption::isAutoSelected() const
    {
        return mAutoSelected;
    }

    bool DialogOption::isAvailable(Dialog* dialog) const
    {
        bool isActive = true;

        if (dialog->getAllProperties()->hasProperty("option" + getId() + "isActive"))
        {
            isActive = dialog->getProperty("option" + getId() + "isActive");
        }
        // if the DialogOption is not active, return false
        if (!isActive)
        {
            return false;
        }
        // if it is active, check it's precondition
        if (mPrecondition)
        {
            return mPrecondition->isTrue(dialog);
        }

        return true;
    }

    void DialogOption::setLabel(const CeGuiString& label)
    {
        mLabel = label;
        mLabel.c_str();
    }

    const CeGuiString& DialogOption::getLabel() const
    {
        return mLabel;
    }

    const CeGuiString& DialogOption::getDefaultPerson() const
    {
        static CeGuiString PLAYER = "player";
        return PLAYER;
    }
}
