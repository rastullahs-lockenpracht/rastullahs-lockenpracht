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
#ifndef __DialogCondition_H__
#define __DialogCondition_H__

#include "AiPrerequisites.h"

namespace rl
{
    class Dialog;
    class DialogVariable;

	class _RlAiExport DialogCondition
    {
    public:
        DialogCondition();
        virtual ~DialogCondition();

        const CeGuiString& getVariableType();

        void setVariable(DialogVariable* var);

        bool isTrue(Dialog* dialog);
        virtual bool isFulfilled(DialogVariable* var, Dialog* dialog) = 0;

    private:
        DialogVariable* mVariable;
    };

    class DialogConditionTrue : public DialogCondition
    {
    public:
        DialogConditionTrue();

        virtual bool isFulfilled(DialogVariable* var, Dialog* dialog);
    };

    class DialogConditionEquals : public DialogCondition
    {
    public:
        DialogConditionEquals(const CeGuiString& testValue);

        virtual bool isFulfilled(DialogVariable* var, Dialog* dialog);

    private:
        CeGuiString mTestValue;
    };

    class DialogConditionInRange : public DialogCondition
    {
    public:
        DialogConditionInRange(Ogre::Real lowerBound, Ogre::Real upperBound);

        virtual bool isFulfilled(DialogVariable* var, Dialog* dialog);
    private:
        Ogre::Real mLowerBound; 
        Ogre::Real mUpperBound;
    };

    class DialogConditionLowerThan : public DialogCondition
    {
    public:
        DialogConditionLowerThan(Ogre::Real upperBound);

        virtual bool isFulfilled(DialogVariable* var, Dialog* dialog);
    private:
        Ogre::Real mUpperBound;
    };

    class DialogConditionGreaterThan : public DialogCondition
    {
    public:
        DialogConditionGreaterThan(Ogre::Real lowerBound);

        virtual bool isFulfilled(DialogVariable* var, Dialog* dialog);
    private:
        Ogre::Real mLowerBound; 
    };

    class DialogConditionLowerOrEquals : public DialogCondition
    {
    public:
        DialogConditionLowerOrEquals(Ogre::Real upperLimit);

        virtual bool isFulfilled(DialogVariable* var, Dialog* dialog);
    private:
        Ogre::Real mUpperLimit;
    };

    class DialogConditionGreaterOrEquals : public DialogCondition
    {
    public:
        DialogConditionGreaterOrEquals(Ogre::Real lowerLimit);

        virtual bool isFulfilled(DialogVariable* var, Dialog* dialog);
    private:
        Ogre::Real mLowerLimit; 
    };
}

#endif // __DialogCondition_H__
