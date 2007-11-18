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

#include "DialogVariable.h"

#include "Creature.h"
#include "Dialog.h"
#include "RulesSubsystem.h"
#include "QuestBook.h"

namespace rl
{
    DialogVariable::DialogVariable()
        : mRecalculate(true)
    {
    }

    DialogVariable::~DialogVariable()
    {
    }

    const Property& DialogVariable::getValue(Dialog* dialog)
    {
        if (mRecalculate)
        {
            mValue = calculateValue(dialog);
            mRecalculate = false;
        }

        return mValue;
    }

    void DialogVariable::invalidate()
    {
        mRecalculate = true;
    }

    DialogPropertyVariable::DialogPropertyVariable(const Ogre::String &propertyName)
        : mPropertyName(propertyName)
    {
    }

    Property DialogPropertyVariable::calculateValue(Dialog* dialog)
    {
        return dialog->getProperty(mPropertyName);
    }

    QuestStateVariable::QuestStateVariable(const Ogre::String &questId, const Ogre::String &propertyName)
        : mQuestId(questId), mPropertyName(propertyName)
    {
    }

    Property QuestStateVariable::calculateValue(Dialog* dialog)
    {
        return RulesSubsystem::getSingleton().getQuestBook()->getQuest(mQuestId)->getProperty(mPropertyName);
    }

    TalentProbeVariable::TalentProbeVariable(const rl::CeGuiString &talent, int modifier)
        : mTalent(talent), mModifier(modifier)
    {
    }

    Property TalentProbeVariable::calculateValue(Dialog* dialog)
    {
        Creature* cr = dialog->getPc(0); ///@todo allow multiple PCs
        return Property(cr->doTalentprobe(mTalent, mModifier));
    }

EigenschaftsProbeVariable::EigenschaftsProbeVariable(const rl::CeGuiString &eigenschaft, int modifier)
        : mEigenschaft(eigenschaft), mModifier(modifier)
    {
    }

    Property EigenschaftsProbeVariable::calculateValue(Dialog* dialog)
    {
        Creature* cr = dialog->getPc(0); ///@todo allow multiple PCs
        return Property(cr->doEigenschaftsprobe(mEigenschaft, mModifier));
    }


}
