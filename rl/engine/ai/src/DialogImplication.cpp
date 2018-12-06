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

#include "DialogImplication.h"

#include "Combat.h"
#include "CombatManager.h"
#include "Dialog.h"
#include "Property.h"
#include "QuestBook.h"
#include "RulesSubsystem.h"

using namespace Ogre;

namespace rl
{

    DialogImplication::DialogImplication()
    {
    }

    DialogImplication::~DialogImplication()
    {
    }

    DialogVariableAssignment::DialogVariableAssignment(const Ogre::String& variableName, const CeGuiString& value)
        : mVariableName(variableName)
        , mValue(value)
    {
    }

    void DialogVariableAssignment::apply(Dialog* dialog)
    {
        Property prop = dialog->getProperty(mVariableName);
        prop.getFromString(mValue);
        dialog->setProperty(mVariableName, prop);
    }

    DialogVariableIncrease::DialogVariableIncrease(const Ogre::String& variableName, const CeGuiString& value)
        : DialogVariableAssignment(variableName, value)
    {
    }

    void DialogVariableIncrease::apply(Dialog* dialog)
    {
        Property prop = dialog->getProperty(mVariableName);
        if (prop.isInt() || prop.isReal())
        {
            Property mod = dialog->getProperty(mVariableName);
            mod.getFromString(mValue);
            Ogre::Real value = prop.getAsNumber() + mod.getAsNumber();
            prop.setValue(value);
            dialog->setProperty(mVariableName, prop);
        }
        else
        {
            LOG_ERROR(Logger::DIALOG,
                "You can not increase the variable '" + mVariableName + "'. The variable's type is not a number");
        }
    }

    DialogVariableDecrease::DialogVariableDecrease(const Ogre::String& variableName, const CeGuiString& value)
        : DialogVariableAssignment(variableName, value)
    {
    }

    void DialogVariableDecrease::apply(Dialog* dialog)
    {
        Property prop = dialog->getProperty(mVariableName);
        if (prop.isInt() || prop.isReal())
        {
            Property mod = dialog->getProperty(mVariableName);
            mod.getFromString(mValue);
            Ogre::Real value = prop.getAsNumber() - mod.getAsNumber();
            prop.setValue(value);
            dialog->setProperty(mVariableName, prop);
        }
        else
        {
            LOG_ERROR(Logger::DIALOG,
                "You can not decrease the variable '" + mVariableName + "'. The variable's type is not a number");
        }
    }

    DialogElementActivation::DialogElementActivation(const CeGuiString& id, bool value, bool isOption)
        : mElementId(id)
        , mValue(value)
        , mIsOption(isOption)
    {
    }

    void DialogElementActivation::apply(Dialog* dialog)
    {
        Property prop;
        if (mIsOption)
        {
            dialog->setProperty("option" + mElementId + "isActive", mValue);
        }
        else
        {
            dialog->setProperty("response" + mElementId + "isActive", mValue);
        }
    }

    void DialogExit::apply(rl::Dialog* dialog)
    {
        dialog->setProperty(Dialog::PROP_EXIT_REQUESTED, true);
    }

    QuestPropertyAssignment::QuestPropertyAssignment(
        const Ogre::String& questId, const Ogre::String& prop, const rl::CeGuiString& newValue)
        : mQuestId(questId)
        , mProperty(prop)
        , mNewValue(newValue)
    {
    }

    void QuestPropertyAssignment::apply(rl::Dialog* dialog)
    {
        Quest* quest = RulesSubsystem::getSingleton().getQuestBook()->getQuest(mQuestId);
        quest->setPropertyAsString(mProperty, mNewValue);
    }

    CombatStart::CombatStart()
    {
    }

    void CombatStart::apply(Dialog* dialog)
    {
        Combat* combat = CombatManager::getSingleton().startCombat();
        ///@FIXME: fix starting combat from dialog
        /*        std::vector<Creature*> allies = dialog->getPlayerCharacters();
                for (std::vector<Creature*>::iterator it = allies.begin(); it != allies.end(); ++it)
                {
                    combat->addAlly(*it);
                }
                std::vector<Creature*> enemies = dialog->getNonPlayerCharacters();
                for (std::vector<Creature*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
                {
                    combat->addOpponent(*it);
                }*/
        combat->start();
    }
}
