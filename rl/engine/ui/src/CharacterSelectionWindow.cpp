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

#include "CharacterSelectionWindow.h"

#include "Creature.h"
#include "PartyManager.h"
#include "RulesMessages.h"

using namespace CEGUI;

namespace rl
{
    CharacterSelectionWindow::CharacterSelectionWindow()
        : AbstractWindow("characterselectionwindow.xml", WIT_MOUSE_INPUT, false)
    {
        mCharacterWindow = getWindow("CharacterSelectionWindow/Characters");
        mCharacterAddedConnection = MessagePump::getSingleton().addMessageHandler<MessageType_PlayerCharAdded>(
            boost::bind(&CharacterSelectionWindow::update, this));
        mCharacterRemovedConnection = MessagePump::getSingleton().addMessageHandler<MessageType_PlayerCharRemoved>(
            boost::bind(&CharacterSelectionWindow::update, this));
    }

    CharacterSelectionWindow::~CharacterSelectionWindow()
    {
        while (0 > mCharacterWindows.size())
        {
            Element* elem = mCharacterWindows.back();
            mCharacterWindows.pop_back();
            mCharacterWindow->removeChild(elem->getWindow());
            delete elem;
        }
    }

    bool CharacterSelectionWindow::update()
    {
        Party party = PartyManager::getSingleton().getCharacters();

        while (party.size() > mCharacterWindows.size())
        {
            Element* elem = new Element(this);
            mCharacterWindow->addChild(elem->getWindow());
            auto size = elem->getWindow()->getPixelSize();
            size_t windowNum = mCharacterWindows.size();
            elem->getWindow()->setPosition(UVector2(UDim(0, 0), UDim(0, windowNum * size.d_height)));
            elem->setVisible(true);
            elem->getWindow()->subscribeEvent(Window::EventMouseClick,
                boost::bind(&CharacterSelectionWindow::handleClickOnCharacter, this, windowNum));

            mCharacterWindows.push_back(elem);
        }

        while (party.size() < mCharacterWindows.size())
        {
            Element* elem = mCharacterWindows.back();
            mCharacterWindows.pop_back();
            mCharacterWindow->removeChild(elem->getWindow());
            delete elem;
        }

        for (unsigned int i = 0; i < party.size(); ++i)
        {
            mCharacterWindows[i]->setCreature(party[i]);
        }

        return true;
    }

    bool CharacterSelectionWindow::handleClickOnCharacter(size_t index)
    {
        Party party = PartyManager::getSingleton().getCharacters();
        PartyManager::getSingleton().setActiveCharacter(party[index]);
        return true;
    }

    CharacterSelectionWindow::Element::Element(CharacterSelectionWindow* parent)
        : AbstractWindow("characterselectionwindow_character.xml", WIT_NONE, false)
        , mParent(parent)
        , mCreature(NULL)
        , mTextName(NULL)
    {
        mTextName = getWindow("CharacterSelectionWindow/Character/Name");
        mPortrait = getWindow("CharacterSelectionWindow/Character/Picture");
        mLP = getProgressBar("CharacterSelectionWindow/Character/LP");
    }

    CharacterSelectionWindow::Element::~Element()
    {
        setCreature(NULL);
    }

    void CharacterSelectionWindow::Element::setCreature(Creature* creature)
    {
        if (mCreature)
        {
            mCreature->removeObjectStateChangeListener(this);
        }

        if (creature)
        {
            creature->addObjectStateChangeListener(this);
            mTextName->setText(creature->getName());
            mPortrait->setProperty("Image", creature->getImageName());
        }
        else
        {
            mTextName->setText("");
        }

        mCreature = creature;
        update();
    }

    void CharacterSelectionWindow::Element::objectStateChanged(ObjectStateChangeEvent* evt)
    {
        update();
    }

    void CharacterSelectionWindow::Element::update()
    {
        float lep;
        if (!mCreature || mCreature->getLeMax() <= 0)
        {
            lep = 0.0;
        }
        else
        {
            lep = (float)mCreature->getLe() / (float)mCreature->getLeMax();
        }
        mLP->setProgress(lep);
    }
}
