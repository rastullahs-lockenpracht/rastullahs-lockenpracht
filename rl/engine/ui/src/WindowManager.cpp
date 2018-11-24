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

#include "WindowManager.h"
#include <CEGUI/WindowManager.h>

#include "AbstractWindow.h"
#include "CoreMessages.h"
#include "CoreSubsystem.h"
#include "Exception.h"
#include "GameLoop.h"
#include "UiSubsystem.h"

template <> rl::WindowManager* Ogre::Singleton<rl::WindowManager>::msSingleton = 0;

using namespace CEGUI;

namespace rl
{

    WindowManager::WindowManager()
        : mWindowList()
        , mNumActiveWindowsMouseInput(0)
        , mNumActiveWindowsKeyboardInput(0)
    {
        mSceneClearConnection = MessagePump::getSingleton().addMessageHandler<MessageType_SceneClearing>(
            boost::bind(&WindowManager::destroyAllWindows, this));
    }

    WindowManager::~WindowManager()
    {
        for (std::list<AbstractWindow*>::iterator it = mWindowList.begin(); it != mWindowList.end(); it++)
        {
            AbstractWindow* window = *it;
            LOG_MESSAGE(Logger::UI, "Delete window " + window->getName());
            // AbstractWindow::getRoot()->removeChild(window->getWindow());
            // CEGUI::WindowManager::getSingleton().destroyWindow(window->getWindow());
        }
        mWindowList.clear();
    }

    void WindowManager::registerWindow(AbstractWindow* window)
    {
        mWindowList.push_back(window);
    }

    void WindowManager::unregisterWindow(AbstractWindow* window)
    {
        mWindowList.remove(window);
    }

    void WindowManager::destroyWindow(AbstractWindow* window)
    {
        delete window;
    }

    void WindowManager::closeTopWindow()
    {
        for (std::list<AbstractWindow*>::iterator it = mWindowList.begin(); it != mWindowList.end(); it++)
        {
            AbstractWindow* cur = *it;
            if (cur->isVisible() && cur->isClosingOnEscape())
            {
                cur->setVisible(false);
                break;
            }
        }
    }

    bool WindowManager::handleMovedToFront(AbstractWindow* window)
    {
        mWindowList.remove(window);
        mWindowList.push_front(window);
        return true;
    }

    bool WindowManager::handleMovedToBack(AbstractWindow* window)
    {
        mWindowList.remove(window);
        mWindowList.push_back(window);
        return true;
    }

    void WindowManager::_visiblityChanged(AbstractWindow* window, bool isVisible)
    {
        if (window->getWindowInputType() & AbstractWindow::WIT_MOUSE_INPUT)
        {
            mNumActiveWindowsMouseInput += isVisible ? +1 : -1;
        }
        if (window->getWindowInputType() & AbstractWindow::WIT_KEYBOARD_INPUT)
        {
            mNumActiveWindowsKeyboardInput += isVisible ? +1 : -1;
        }

        // TODO
        //        if (mNumActiveWindowsMouseInput == 1)
        //        {
        //            CEGUI::MouseCursor::getSingleton().show();
        //        }
        //        else if (mNumActiveWindowsMouseInput == 0)
        //        {
        //            CEGUI::MouseCursor::getSingleton().hide();
        //        }
    }

    int WindowManager::getWindowInputMask() const
    {
        int mask = 0;
        mask |= mNumActiveWindowsMouseInput > 0 ? AbstractWindow::WIT_MOUSE_INPUT : 0;
        mask |= mNumActiveWindowsKeyboardInput > 0 ? AbstractWindow::WIT_KEYBOARD_INPUT : 0;
        return mask;
    }

    AbstractWindow* WindowManager::getActiveWindow()
    {
        // iterate through windows
        for (std::list<AbstractWindow*>::iterator it = mWindowList.begin(); it != mWindowList.end(); it++)
        {
            if ((*it)->getWindow()->isActive())
            {
                return *it;
            }
        }

        return NULL;
    }

    bool WindowManager::destroyAllWindows()
    {
        for (std::list<AbstractWindow*>::iterator it = mWindowList.begin(); it != mWindowList.end(); it++)
        {
            AbstractWindow* cur = *it;
            //			if (cur->isVisible())
            //			{
            cur->setVisible(false, true);
            //			}
            //          else
            //          {
            //              delete cur;
            //          }
        }

        mWindowList.clear();
        return true;
    }
}
