/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include "DialogCondition.h"

#include "Dialog.h"
#include "DialogVariable.h"
#include "Property.h"

namespace rl
{

    DialogCondition::DialogCondition()
        : mVariable(NULL)
    {
    }

    DialogCondition::~DialogCondition()
    {
    }

    bool DialogCondition::isTrue(Dialog* dialog)
    {
        if (mVariable)
        {
            mVariable->invalidate();
            return isFulfilled(mVariable, dialog);
        }
        return false;
    }

    void DialogCondition::setVariable(DialogVariable* var)
    {
        mVariable = var;
    }

    DialogConditionTrue::DialogConditionTrue() 
    {
    }

    DialogConditionTrue::~DialogConditionTrue() 
    {
    }

    bool DialogConditionTrue::isFulfilled(DialogVariable* var, Dialog* dialog) 
    { 
        return true; 
    }

    DialogConditionEquals::DialogConditionEquals(const CeGuiString& testValue)
        : mTestValue(testValue)
    {
    }

    DialogConditionEquals::~DialogConditionEquals()
    {
    }

    bool DialogConditionEquals::isFulfilled(DialogVariable* var, Dialog* dialog)
    {
        return var->getValue(dialog).getAsString() == mTestValue;
    }

    DialogConditionInRange::DialogConditionInRange(Ogre::Real lowerBound, Ogre::Real upperBound)
    {
    }
    
    DialogConditionInRange::~DialogConditionInRange()
    {
    }
    
    bool DialogConditionInRange::isFulfilled(DialogVariable* var, Dialog* dialog)
    {
        Ogre::Real numVar = var->getValue(dialog).getAsNumber();
        return mLowerBound <= numVar && numVar <= mUpperBound;
    }

    DialogConditionLowerThan::DialogConditionLowerThan(Ogre::Real upperBound)
        : mUpperBound(upperBound)
    {
    }
    
    DialogConditionLowerThan::~DialogConditionLowerThan()
    {
    }

    bool DialogConditionLowerThan::isFulfilled(DialogVariable* var, Dialog* dialog)
    {
        return var->getValue(dialog).getAsNumber() < mUpperBound;
    }

    DialogConditionGreaterThan::DialogConditionGreaterThan(Ogre::Real lowerBound)
        : mLowerBound(lowerBound)
    {
    }

    DialogConditionGreaterThan::~DialogConditionGreaterThan()
    {
    }

    bool DialogConditionGreaterThan::isFulfilled(DialogVariable* var, Dialog* dialog)
    {
        return var->getValue(dialog).getAsNumber() > mLowerBound;
    }

}
