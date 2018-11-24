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

#include "CombatGui.h"

#include "Combatant.h"
#include "CoreSubsystem.h"
#include "CreatureController.h"
#include "CreatureControllerManager.h"
#include "RulesMessages.h"
#include "UiMessages.h"
#include "WindowFactory.h"
#include "World.h"

using namespace Ogre;

namespace rl
{

    CombatGui::CombatGui(Combat* combat, CameraObject* camera)
        : mCombatWindow(NULL)
        , mGameLoggerWindow(NULL)
        , mOpponentButtons()
        , mCombat(combat)
        , mCamera(camera)
        , mUserInputEnabled(false)
        , mGameLoggerWindowOpened(false)
        , mVisible(false)
    {
        mCombatWindow = WindowFactory::getSingleton().getCombatWindow();
        mGameLoggerWindow = WindowFactory::getSingleton().getGameLogger();

        // Message handlers
        mEnemyButtonClickedConnection
            = MessagePump::getSingleton().addMessageHandler<MessageType_CombatIoEnemyButtonClicked>(
                boost::bind(&CombatGui::enemyButtonClicked, this, _1, _2));
        mEndTurnButtonClickedConnection
            = MessagePump::getSingleton().addMessageHandler<MessageType_CombatIoEndTurnButtonClicked>(
                boost::bind(&CombatGui::endTurnButtonClicked, this));
        mEnemyEnteredCombatConnection
            = MessagePump::getSingleton().addMessageHandler<MessageType_CombatOpponentEntered>(
                boost::bind(&CombatGui::enemyEnteredCombat, this, _1));
        mEnemyLeftCombatConnection = MessagePump::getSingleton().addMessageHandler<MessageType_CombatOpponentLeft>(
            boost::bind(&CombatGui::enemyLeftCombat, this, _1));

        // Initialise HUD-MO. Put it into 2D mode and make sure it is always rendered.
        SceneManager* sceneMgr = CoreSubsystem::getSingleton().getWorld()->getSceneManager();
        mHud = sceneMgr->createManualObject("__COMBAT_HUD__");
        mHud->setUseIdentityProjection(true);
        mHud->setUseIdentityView(true);
        AxisAlignedBox infiniteAabb;
        infiniteAabb.setInfinite();
        mHud->setBoundingBox(infiniteAabb);
        mHud->setRenderQueueGroup(RENDER_QUEUE_OVERLAY);
        sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mHud);

        // Create an attack/parry/(goto) button set for all opponents present at the beginning.
        const Combat::CombatantSet& opponents = mCombat->getAllPlayerOpponents();
        for (Combat::CombatantSet::const_iterator it = opponents.begin(), end = opponents.end(); it != end; ++it)
        {
            int handle = mCombatWindow->addEnemyButtonSet();
            mOpponentButtons.insert(std::make_pair(*it, handle));
        }

        enableUserInput(false);
    }

    CombatGui::~CombatGui()
    {
        hide();
        mHud->getParentSceneNode()->detachObject(mHud);
        SceneManager* sceneMgr = CoreSubsystem::getSingleton().getWorld()->getSceneManager();
        sceneMgr->destroyManualObject(mHud);
    }

    void CombatGui::update()
    {
        int screenWidth = mCamera->getCamera()->getViewport()->getActualWidth();
        int screenHeight = mCamera->getCamera()->getViewport()->getActualHeight();
        CombatWindow::SetSize setSize = mCombatWindow->getButtonSetSize();

        // update HUD
        mHud->clear();

        mHud->begin("alpha_red", RenderOperation::OT_LINE_LIST);
        const Combat::CombatantSet& opponents = mCombat->getAllPlayerOpponents();
        for (Combat::CombatantSet::const_iterator it = opponents.begin(), end = opponents.end(); it != end; ++it)
        {
            Ogre::FloatRect rec = getScreenRectFromWorldAabb(
                (*it)->getCreatureController()->getCreature()->getActor()->_getSceneNode()->_getWorldAABB());

            // Place buttons, update every frame (camera movement requires this)
            // TODO: is there any better possibility (attach button-windows to ogre node?!?)
            // if (mUserInputEnabled)
            //{
            Ogre::Rect screenRect = mCamera->getPixelRectOnScreen(rec);
            // Place button set above opponent, horizontally centered.
            int xpos = screenRect.left + ((screenRect.right - screenRect.left) - setSize.first) / 2;
            mCombatWindow->placeEnemyButtonSet(mOpponentButtons[*it], xpos, screenRect.top - setSize.second);
            //}

            // draw Hud

            Real fragWidth = fabs(rec.right - rec.left) / 6.0;
            Real fragHeight = fabs(rec.bottom - rec.top) / 6.0;
            Real length = std::min(fragWidth, fragHeight);

            mHud->position(rec.left, rec.top, 0.0f);
            mHud->position(rec.left + length, rec.top, 0.0f);
            mHud->position(rec.left, rec.top - length, 0.0f);

            mHud->position(rec.right, rec.top, 0.0f);
            mHud->position(rec.right - length, rec.top, 0.0f);
            mHud->position(rec.right, rec.top - length, 0.0f);

            mHud->position(rec.left, rec.bottom, 0.0f);
            mHud->position(rec.left + length, rec.bottom, 0.0f);
            mHud->position(rec.left, rec.bottom + length, 0.0f);

            mHud->position(rec.right, rec.bottom, 0.0f);
            mHud->position(rec.right - length, rec.bottom, 0.0f);
            mHud->position(rec.right, rec.bottom + length, 0.0f);

            mHud->index(0);
            mHud->index(1);
            mHud->index(0);
            mHud->index(2);

            mHud->index(3);
            mHud->index(4);
            mHud->index(3);
            mHud->index(5);

            mHud->index(6);
            mHud->index(7);
            mHud->index(6);
            mHud->index(8);

            mHud->index(9);
            mHud->index(10);
            mHud->index(9);
            mHud->index(11);
        }
        mHud->end();
    }

    void CombatGui::show()
    {
        if (!mVisible)
        {
            mGameLoggerWindowOpened = !mGameLoggerWindow->isVisible();
            mCombatWindow->setVisible(true);
            mGameLoggerWindow->setVisible(true);
            mHud->setVisible(true);
            mVisible = true;
        }
    }

    void CombatGui::hide()
    {
        if (mVisible)
        {
            mCombatWindow->setVisible(false);
            mGameLoggerWindow->setVisible(!mGameLoggerWindowOpened);
            mHud->setVisible(false);
            mVisible = false;
        }
    }

    bool CombatGui::enemyButtonClicked(int handle, int buttonIndex)
    {
        for (OpponentButtonsMap::const_iterator it = mOpponentButtons.begin(); it != mOpponentButtons.end(); ++it)
        {
            if (it->second == handle)
            {
                if (buttonIndex == CombatWindow::ATTACK_BUTTON)
                {
                    MessagePump::getSingleton().sendMessage<MessageType_CombatIoAttackOpponent>(it->first);
                    mCombatWindow->setAttackedOpponentName(it->first->getName());
                }
                else if (buttonIndex == CombatWindow::PARRY_BUTTON)
                {
                    MessagePump::getSingleton().sendMessage<MessageType_CombatIoParryOpponent>(it->first);
                    mCombatWindow->setParriedOpponentName(it->first->getName());
                }
                break;
            }
        }

        return true;
    }

    bool CombatGui::endTurnButtonClicked()
    {
        MessagePump::getSingleton().sendMessage<MessageType_CombatIoEndTurnRequested>();
        return true;
    }

    bool CombatGui::enemyEnteredCombat(Combatant* combatant)
    {
        int handle = mCombatWindow->addEnemyButtonSet();
        mCombatWindow->enableEnemyButtonSet(handle, mUserInputEnabled);
        mOpponentButtons.insert(std::make_pair(combatant, handle));
        return true;
    }

    bool CombatGui::enemyLeftCombat(Combatant* combatant)
    {
        OpponentButtonsMap::iterator it = mOpponentButtons.find(combatant);
        if (it != mOpponentButtons.end())
        {
            mCombatWindow->removeEnemyButtonSet(it->second);
            mOpponentButtons.erase(it);
        }
        return true;
    }

    Ogre::FloatRect CombatGui::getScreenRectFromWorldAabb(const AxisAlignedBox& aabb) const
    {
        // Initialise each side with the value of the opposite side, so that min/max work smoothly.
        Real left = 1.0f, bottom = 1.0f, right = -1.0f, top = -1.0f;

        // Determine screen pos of all corners and widen the rect if needed.
        const auto& corners = aabb.getAllCorners();
        for (size_t i = 0; i < 8; ++i)
        {
            Vector3 screenSpacePos = mCamera->getPointOnScreen(corners[i]);
            if (screenSpacePos.z > 0)
                continue; // Behind camera

            left = std::min(left, screenSpacePos.x);
            right = std::max(right, screenSpacePos.x);
            bottom = std::min(bottom, screenSpacePos.y);
            top = std::max(top, screenSpacePos.y);
        }

        return FloatRect(left, top, right, bottom);
    }

    void CombatGui::enableUserInput(bool enable)
    {
        mUserInputEnabled = enable;

        // Hide or show combatant buttons
        mCombatWindow->enableAllEnemyButtonSets(enable);
    }
}
