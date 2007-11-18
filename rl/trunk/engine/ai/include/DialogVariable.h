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
#ifndef __DialogVariable_H__
#define __DialogVariable_H__

#include "AiPrerequisites.h"

#include "Property.h"

namespace rl
{
    class Dialog;

	class _RlAiExport DialogVariable
    {
    public:
        const Property& getValue(Dialog* dialog);
        void invalidate();

    protected:
        DialogVariable();
        virtual ~DialogVariable();
        virtual Property calculateValue(Dialog* dialog) = 0;

    private:
        bool mRecalculate;
        Property mValue;
    };

    class DialogPropertyVariable : public DialogVariable
    {
    public:
        DialogPropertyVariable(const Ogre::String& propertyName);

    protected:
        virtual Property calculateValue(Dialog* dialog);

    private:
        Ogre::String mPropertyName;
    };

    class QuestStateVariable : public DialogVariable
    {
    public:
        QuestStateVariable(const Ogre::String& questId, const Ogre::String& propertyName);

    protected:
        virtual Property calculateValue(Dialog* dialog);

    private:
        Ogre::String mQuestId;
        Ogre::String mPropertyName;
    };

    class EigenschaftsProbeVariable : public DialogVariable
    {
    public:
        EigenschaftsProbeVariable(const CeGuiString& eigenschaft, int modifier);

    protected:
        virtual Property calculateValue(Dialog* dialog);

    private:
        CeGuiString mEigenschaft;
        int mModifier;
    };

    class TalentProbeVariable : public DialogVariable
    {
    public:
        TalentProbeVariable(const CeGuiString& talent, int modifier);
        
    protected:
        virtual Property calculateValue(Dialog* dialog);

    private:
        CeGuiString mTalent;
        int mModifier;
    };
}

#endif //__DialogVariable_H__
