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

#include "DialogOption.h"

#include "DialogCondition.h"

namespace rl
{

    DialogOption::DialogOption(int id, bool isAutoSelected)
    : DialogElement(id), mPrecondition(NULL), mResponse(NULL), mLabel(""), mIsAutoSelected(isAutoSelected)
    {
    }

    DialogOption::~DialogOption()
    {
    }

    void DialogOption::setResponse(DialogResponse *response)
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
        RlAssert(mPrecondition, "No precondition found for option with id: " 
                                + Ogre::StringConverter::toString(getId())
                                + "\nA precondition must be set to get its variable type");
        return mPrecondition->getVariableType();
    }

    bool DialogOption::isConditional()
    {
        return (mPrecondition != NULL);
    }

    bool DialogOption::isAutoSelected() const
    {
        return mIsAutoSelected;
    }

    bool DialogOption::isAvailable(Dialog* dialog) const
    {
        if (mPrecondition)
        {
            return mPrecondition->isTrue(dialog);
        }

        return true;
    }

    void DialogOption::setLabel(const CeGuiString &label)
    {
        mLabel = label;
        mLabel.c_str();
    }

    const CeGuiString& DialogOption::getLabel() const
    {
        return mLabel;
    }

}
