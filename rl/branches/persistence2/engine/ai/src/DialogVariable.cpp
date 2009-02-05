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

#include "DialogVariable.h"

#include "Creature.h"
#include "Dialog.h"
#include "RulesSubsystem.h"
#include "QuestBook.h"

namespace rl
{
    DialogVariable::DialogVariable(const CeGuiString& type)
        : mRecalculate(true), mType(type)
    {
    }

    DialogVariable::~DialogVariable()
    {
    }

    const CeGuiString& DialogVariable::getType()
    {
        return mType;
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
        : DialogVariable("dialogvariable"), mPropertyName(propertyName)
    {
    }

    Property DialogPropertyVariable::calculateValue(Dialog* dialog)
    {
        return dialog->getProperty(mPropertyName);
    }

    QuestStateVariable::QuestStateVariable(const Ogre::String &questId, const Ogre::String &propertyName)
        : DialogVariable("queststate"), mQuestId(questId), mPropertyName(propertyName)
    {
    }

    Property QuestStateVariable::calculateValue(Dialog* dialog)
    {
        return RulesSubsystem::getSingleton().getQuestBook()->getQuest(mQuestId)->getProperty(mPropertyName);
    }

    TalentProbeVariable::TalentProbeVariable(const rl::CeGuiString &talent, int modifier, const rl::CeGuiString& target)
        : DialogVariable("talentcheck"), mTalent(talent), mTarget(target), mModifier(modifier)
    {
    }

    Property TalentProbeVariable::calculateValue(Dialog* dialog)
    {
        Creature* cr = NULL;
        if(mTarget == "pc")
        {
            cr = dialog->getPc(0); ///@todo allow multiple PCs
        }
        else if (mTarget == "npc")
        {
            cr = dialog->getNpc(0);
        }
        // if no target was given, use the player character. 
        // @todo: remove this, target should be required!
        if(cr == NULL) { cr = dialog->getPc(0);}
        return Property(cr->doTalentprobe(mTalent, mModifier));
    }

    EigenschaftsProbeVariable::EigenschaftsProbeVariable(const rl::CeGuiString &eigenschaft, int modifier, const rl::CeGuiString& target)
        : DialogVariable("attributecheck"), mEigenschaft(eigenschaft), mTarget(target), mModifier(modifier)
    {
    }

    Property EigenschaftsProbeVariable::calculateValue(Dialog* dialog)
    {
        Creature* cr = NULL;
        if(mTarget == "pc")
        {
            cr = dialog->getPc(0); ///@todo allow multiple PCs
        }
        else if (mTarget == "npc")
        {
            cr = dialog->getNpc(0);
        }
        // if no target was given, use the player character. 
        // @todo: remove this, target should be required!
        if(cr == NULL) { cr = dialog->getPc(0);}
        return Property(cr->doEigenschaftsprobe(mEigenschaft, mModifier));
    }
	
	RandomVariable::RandomVariable(int maximum)
		: DialogVariable("random"), mMaximum(maximum)
	{
	}
	
	Property RandomVariable::calculateValue(Dialog* dialog)
	{
		double d = std::rand();
        return Property(static_cast<int>(d * mMaximum / RAND_MAX) + 1);
	}
}
