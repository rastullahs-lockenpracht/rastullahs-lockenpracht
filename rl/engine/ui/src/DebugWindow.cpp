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

#include "UiPrerequisites.h"

#include "DebugWindow.h"
#include "GameLoop.h"
#include "InputManager.h"
#include "UiSubsystem.h"

#include "Actor.h"
#include "ActorManager.h"
#include "CameraObject.h"
#include "CoreSubsystem.h"
#include "Creature.h"
#include "Exception.h"
#include "MeshObject.h"
#include "PartyManager.h"

using namespace CEGUI;
using namespace Ogre;

template <> rl::DebugWindow* Ogre::Singleton<rl::DebugWindow>::msSingleton = 0;

namespace rl
{
    DebugWindow::DebugWindow()
        : AbstractWindow("debugwindow.xml", WIT_NONE)
        , mMessageText()
        , mPageCaption()
        , mPageText()
        , mPageTexts()
        , mCurrentPage(Ogre::BLANKSTRING)
        , mDebugPageName("General Informations")
    {
        mPageCaption = getWindow("DebugWindow/PageCaption");
        mPageCaption->moveToFront();

        mPageText = getMultiLineEditbox("DebugWindow/PageText");

        mMessageText = getWindow("DebugWindow/MessageText");
        mMessageText->moveToFront();

        bindDestroyWindowToXButton();
        registerPage("General Informations");
    }

    DebugWindow::~DebugWindow()
    {
        unregisterPage("General Informations");
    }

    void DebugWindow::registerPage(const Ogre::String& page)
    {
        if (mPageTexts.find(page) != mPageTexts.end())
        {
            LOG_ERROR("DebugWindow", page + " already registered as a page.");
        }
        else
        {
            mPageTexts.insert(make_pair(page, Ogre::BLANKSTRING));
        }

        // If this is the first page, activate it
        if (mCurrentPage == Ogre::BLANKSTRING)
        {
            mCurrentPage = page;
        }
    }

    void DebugWindow::unregisterPage(const Ogre::String& page)
    {
        PageTextMap::iterator it = mPageTexts.find(page);
        if (it != mPageTexts.end())
        {
            mPageTexts.erase(it);
        }
        else
        {
            Throw(IllegalArgumentException, page + " is not registered as a page.");
        }

        // If this is the current page, switch over to the next
        // or clear text, if no other page is left.
        if (mCurrentPage == page && !mPageTexts.empty())
        {
            mCurrentPage = page;
        }
        else if (mCurrentPage == page)
        {
            mCurrentPage = Ogre::BLANKSTRING;
        }
        updatePageText();
    }

    void DebugWindow::setPageText(const Ogre::String& page, const Ogre::String& text)
    {
        PageTextMap::iterator it = mPageTexts.find(page);
        if (it == mPageTexts.end())
        {
            LOG_ERROR("DebugWindow", page + " is not registered as a page.");
        }
        else
        {
            it->second = text;
        }

        if (mCurrentPage == page)
        {
            updatePageText();
        }
    }

    void DebugWindow::setMessageText(const Ogre::String& text)
    {
        CeGuiString o(text.c_str());
        mMessageText->setText(o);
    }

    void DebugWindow::showNextPage()
    {
        PageTextMap::iterator it = mPageTexts.find(mCurrentPage);
        if (it != mPageTexts.end())
        {
            ++it;
            if (it == mPageTexts.end())
            {
                it = mPageTexts.begin();
            }
            mCurrentPage = it->first;
            updatePageText();
        }
        else
        {
            RlAssert1(mPageTexts.empty());
        }
    }

    void DebugWindow::run(Ogre::Real elapsedTime)
    {
        updateFps();
    }

    void DebugWindow::updatePageText()
    {
        Ogre::String text = mCurrentPage == Ogre::BLANKSTRING ? Ogre::BLANKSTRING : mPageTexts[mCurrentPage];
        mPageCaption->setText(mCurrentPage);
        mPageText->setText(text);
    }

    void DebugWindow::updateFps()
    {
        const Ogre::RenderTarget::FrameStats& stats = CoreSubsystem::getSingleton().getRenderWindow()->getStatistics();

        Ogre::String textSt = "Current FPS: " + StringConverter::toString(stats.lastFPS) + "\nBest/worst/avg FPS: "
            + StringConverter::toString(stats.bestFPS) + "/" + StringConverter::toString(stats.worstFPS) + "/"
            + StringConverter::toString(stats.avgFPS) + "\nBest/worst Frame times: "
            + StringConverter::toString(stats.bestFPS) + "/" + StringConverter::toString(stats.worstFPS)
            + "\nTriangle Count: " + StringConverter::toString(stats.triangleCount);

        if (PartyManager::getSingleton().getActiveCharacter() != NULL
            && PartyManager::getSingleton().getActiveCharacter()->getActor() != NULL)
        {
            ActorControlledObject* charObj
                = PartyManager::getSingletonPtr()->getActiveCharacter()->getActor()->getControlledObject();

            if (charObj != NULL)
            {
                Ogre::Vector3 pos = charObj->getMovableObject()->getParentNode()->_getDerivedPosition();
                Ogre::Quaternion ori = charObj->getMovableObject()->getParentNode()->_getDerivedOrientation();

                textSt += "\nPlayer Position [ " + StringConverter::toString(pos.x, 2, 0, 32, std::ios_base::fixed)
                    + ", " + StringConverter::toString(pos.y, 2, 0, 32, std::ios_base::fixed) + ", "
                    + StringConverter::toString(pos.z, 2, 0, 32, std::ios_base::fixed) + " ]";

                textSt += "\nPlayer Orientation [ " + StringConverter::toString(ori.w, 4, 0, 32, std::ios_base::fixed)
                    + ", " + StringConverter::toString(ori.x, 2, 0, 32, std::ios_base::fixed) + ", "
                    + StringConverter::toString(ori.y, 2, 0, 32, std::ios_base::fixed) + ", "
                    + StringConverter::toString(ori.z, 2, 0, 32, std::ios_base::fixed) + " ]";
            }
        }

        Actor* camActor = ActorManager::getSingleton().getActor("DefaultCamera");
        if (camActor != 0 && camActor->_getSceneNode() != NULL)
        {
            Ogre::Camera* cam = dynamic_cast<CameraObject*>(camActor->getControlledObject())->getCamera();
            Ogre::Vector3 pos = cam->getDerivedPosition();

            textSt += "\nCamera Position [ " + StringConverter::toString(pos.x, 2, 0, 32, std::ios_base::fixed) + ", "
                + StringConverter::toString(pos.y, 2, 0, 32, std::ios_base::fixed) + ", "
                + StringConverter::toString(pos.z, 2, 0, 32, std::ios_base::fixed) + " ]";
        }

        setPageText(mDebugPageName, textSt);
    }

    void DebugWindow::setVisible(bool visible, bool destroyAfterHide)
    {
        AbstractWindow::setVisible(visible, destroyAfterHide);
        if (visible)
            GameLoop::getSingleton().addTask(this, GameLoop::TG_GRAPHICS);
        else
            GameLoop::getSingleton().removeTask(this);
    }

    const Ogre::String& DebugWindow::getName() const
    {
        static Ogre::String NAME = "DebugWindow";

        return NAME;
    }
}
