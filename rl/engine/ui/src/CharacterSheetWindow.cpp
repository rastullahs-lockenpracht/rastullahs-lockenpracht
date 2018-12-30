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

#include "CharacterSheetWindow.h"

#include <CEGUI/PropertyHelper.h>
#include <CEGUI/widgets/ListboxTextItem.h>
#include <boost/bind.hpp>

#include "Creature.h"
#include "DsaManager.h"
#include "InputManager.h"
#include "Talent.h"

#include "ObjectStateChangeEvent.h"

using namespace CEGUI;
using namespace Ogre;

namespace rl
{

    CharacterSheetWindow::CharacterSheetWindow()
        : AbstractWindow("charactersheet.xml", WIT_MOUSE_INPUT)
        , mCharacter(NULL)
    {
        mTalentTable = getMultiColumnList("CharacterSheet/TalentSheet/Table");
        mTalentTable->addColumn("Talent", 0, cegui_reldim(0.5));
        mTalentTable->addColumn("Probe", 1, cegui_reldim(0.3));
        mTalentTable->addColumn("eBE", 2, cegui_reldim(0.1));
        mTalentTable->addColumn("TW", 3, cegui_reldim(0.1));
        mTalentTable->setUserSortControlEnabled(false);

        mLE = getWindow("CharacterSheet/CharacterSheet/LE");
        mAE = getWindow("CharacterSheet/CharacterSheet/AE");
        mAP = getWindow("CharacterSheet/CharacterSheet/AP");
        mName = getWindow("CharacterSheet/CharacterSheet/Name");
        mRasse = getWindow("CharacterSheet/CharacterSheet/Rasse");
        mKultur = getWindow("CharacterSheet/CharacterSheet/Kultur");
        mProfession = getWindow("CharacterSheet/CharacterSheet/Profession");
        mEffekte = getWindow("CharacterSheet/CharacterSheet/Effekte");

        bindHideWindowToXButton();

        /*for (int i=0; i<EIGENSCHAFT_COUNT; i++)
            mEigenschaft[i] =
                getWindow(("CharacterSheet/CharacterSheet/Eigenschaften/"+
                    DsaManager::getSingleton().getEigenschaft(i)->getNameAbbreviation()).c_str());*/
        mEigenschaft[0] = getWindow("CharacterSheet/CharacterSheet/Eigenschaften/MU");
        mEigenschaft[1] = getWindow("CharacterSheet/CharacterSheet/Eigenschaften/KL");
        mEigenschaft[2] = getWindow("CharacterSheet/CharacterSheet/Eigenschaften/IN");
        mEigenschaft[3] = getWindow("CharacterSheet/CharacterSheet/Eigenschaften/CH");
        mEigenschaft[4] = getWindow("CharacterSheet/CharacterSheet/Eigenschaften/FF");
        mEigenschaft[5] = getWindow("CharacterSheet/CharacterSheet/Eigenschaften/GE");
        mEigenschaft[6] = getWindow("CharacterSheet/CharacterSheet/Eigenschaften/KO");
        mEigenschaft[7] = getWindow("CharacterSheet/CharacterSheet/Eigenschaften/KK");
    }

    CharacterSheetWindow::~CharacterSheetWindow()
    {
        setCharacter(NULL);
    }

    void CharacterSheetWindow::setCharacter(Creature* creature)
    {
        if (creature == NULL)
        {
            if (mCharacter != NULL)
            {
                mCharacter->removeObjectStateChangeListener(this);
            }
        }
        else
        {
            creature->addObjectStateChangeListener(this);
            mCharacter = creature;
            update();
        }
    }

    void CharacterSheetWindow::update()
    {
        ///@todo: Daten updaten
        updateTalents();
        /*if (mCharacter->isMagic())
            updateMagic();*/
        updateValues();
    }

    void CharacterSheetWindow::updateValues()
    {
        mName->setText("Name: " + mCharacter->getName());
        mRasse->setText(""); // bei Tieren...
        mKultur->setText("");
        mProfession->setText("");
        mEffekte->setText("Effekte:\n" + mCharacter->getEffects());

        mLE->setText("LeP: " + StringConverter::toString(mCharacter->getLe()) + "/"
            + StringConverter::toString(mCharacter->getLeMax()));

        /*for (unsigned int eig = 0; eig < EIGENSCHAFT_COUNT; eig++)
        {
            CeGuiString name = DsaManager::getSingleton().getEigenschaft(eig)->getName();
            mEigenschaft[eig]->setText(
                name +": "+
                StringConverter::toString(mCharacter->getEigenschaft(eig)));
        }*/
        mEigenschaft[0]->setText(DsaManager::getSingleton().getEigenschaft(E_MUT)->getName() + ": "
            + StringConverter::toString(mCharacter->getEigenschaft(E_MUT)));
        mEigenschaft[1]->setText(DsaManager::getSingleton().getEigenschaft(E_KLUGHEIT)->getName() + ": "
            + StringConverter::toString(mCharacter->getEigenschaft(E_KLUGHEIT)));
        mEigenschaft[2]->setText(DsaManager::getSingleton().getEigenschaft(E_INTUITION)->getName() + ": "
            + StringConverter::toString(mCharacter->getEigenschaft(E_INTUITION)));
        mEigenschaft[3]->setText(DsaManager::getSingleton().getEigenschaft(E_CHARISMA)->getName() + ": "
            + StringConverter::toString(mCharacter->getEigenschaft(E_CHARISMA)));
        mEigenschaft[4]->setText(DsaManager::getSingleton().getEigenschaft(E_FINGERFERTIGKEIT)->getName() + ": "
            + StringConverter::toString(mCharacter->getEigenschaft(E_FINGERFERTIGKEIT)));
        mEigenschaft[5]->setText(DsaManager::getSingleton().getEigenschaft(E_GEWANDTHEIT)->getName() + ": "
            + StringConverter::toString(mCharacter->getEigenschaft(E_GEWANDTHEIT)));
        mEigenschaft[6]->setText(DsaManager::getSingleton().getEigenschaft(E_KONSTITUTION)->getName() + ": "
            + StringConverter::toString(mCharacter->getEigenschaft(E_KONSTITUTION)));
        mEigenschaft[7]->setText(DsaManager::getSingleton().getEigenschaft(E_KOERPERKRAFT)->getName() + ": "
            + StringConverter::toString(mCharacter->getEigenschaft(E_KOERPERKRAFT)));

        for (unsigned int row = 0; row < mTalentTable->getRowCount(); row++)
        {
            int tw = mCharacter->getTalent(mTalentTable->getItemAtGridReference(MCLGridRef(row, 0))->getText());

            mTalentTable->getItemAtGridReference(MCLGridRef(row, 3))->setText(StringConverter::toString(tw));
        }
    }

    void CharacterSheetWindow::updateTalents()
    {
        const Creature::TalentMap talents = mCharacter->getAllTalents();

        while (mTalentTable->getRowCount() > talents.size())
            mTalentTable->removeRow(mTalentTable->getRowCount() - 1);
        while (mTalentTable->getRowCount() < talents.size())
            mTalentTable->addRow();

        int talentNum = 0;

        for (Creature::TalentMap::const_iterator iter = talents.begin(); iter != talents.end(); iter++)
        {
            // Talente in die Talenttabelle
            Talent* talent = DsaManager::getSingleton().getTalent((*iter).first);
            mTalentTable->setItem(new ListboxTextItem(talent->getName()), 0, talentNum);

            EigenschaftTripel eigensch = talent->getEigenschaften();
            CeGuiString probe;
            probe = DsaManager::getSingleton().getEigenschaft(eigensch.first)->getNameAbbreviation() + "/";
            probe += DsaManager::getSingleton().getEigenschaft(eigensch.second)->getNameAbbreviation() + "/";
            probe += DsaManager::getSingleton().getEigenschaft(eigensch.third)->getNameAbbreviation();

            mTalentTable->setItem(new ListboxTextItem(probe), 1, talentNum);
            CeGuiString eBe;
            if (talent->getEbe() == EBE_KEINE_BE)
                eBe = "-";
            else if (talent->getEbe() == EBE_BEx2)
                eBe = "BEx2";
            else if (talent->getEbe() == 0)
                eBe = "BE";
            else if (talent->getEbe() > 0)
                eBe = "BE+" + CEGUI::PropertyHelper<int>::toString(talent->getEbe());
            else
                eBe = "BE" + CEGUI::PropertyHelper<int>::toString(talent->getEbe());

            mTalentTable->setItem(new ListboxTextItem(eBe), 2, talentNum);
            mTalentTable->setItem(new ListboxTextItem(""), 3, talentNum);

            talentNum++;
        }
    }

    void CharacterSheetWindow::objectStateChanged(ObjectStateChangeEvent* evt)
    {
        update();
    }

    bool CharacterSheetWindow::handleClose()
    {
        setCharacter(NULL);
        setVisible(false);

        return true;
    }
}
