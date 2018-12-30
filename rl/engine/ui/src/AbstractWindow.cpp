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

#include <CEGUI/GUIContext.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/widgets/FrameWindow.h>
#include <boost/bind.hpp>

#include "Exception.h"
#include "UiPrerequisites.h"

#include "AbstractWindow.h"
#include "CeGuiHelper.h"
#include "JobScheduler.h"
#include "UiSubsystem.h"
#include "WindowFadeJob.h"
#include "WindowManager.h"

using namespace std;
using namespace CEGUI;
using namespace Ogre;

namespace rl
{

    int AbstractWindow::sNumAbstractWindows = 0;

    AbstractWindow::AbstractWindow(const CeGuiString& xmlfile, int inputType, bool closeOnEscape, bool modal)
        : mVisible(false)
        , mModal(modal)
        , mWindowInputType(inputType)
        , mCloseOnEscape(closeOnEscape)
    {
        LOG_MESSAGE(Logger::UI, "Lade Fenster '" + Ogre::String(xmlfile.c_str()) + "'");
        mWindow = AbstractWindow::loadWindow(xmlfile, mNamePrefix);
        if (mWindow == NULL)
        {
            Throw(rl::IllegalStateException, Ogre::String("Could not load window '") + xmlfile.c_str() + "'.");
        }

        CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(mWindow);

        if (modal)
        {
            mWindow->setModalState(true);
            mWindow->setAlwaysOnTop(true);
            mWindow->moveToFront();
            mWindow->show();
        }
        else
        {
            mWindow->hide();
        }

        mNormalAlpha = mWindow->getAlpha();
        mName = mWindow->getName();
        WindowManager::getSingleton().registerWindow(this);
        mWindow->subscribeEvent(Window::EventActivated,
            boost::bind(&rl::WindowManager::handleMovedToFront, rl::WindowManager::getSingletonPtr(), this));
    }

    AbstractWindow::~AbstractWindow()
    {
        mWindow->hide();
        mWindow->removeAllEvents();
        WindowManager::getSingleton().unregisterWindow(this);
        CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->removeChild(mWindow);
        CEGUI::WindowManager::getSingleton().destroyWindow(mWindow);
    }

    CEGUI::Window* AbstractWindow::loadWindow(const CeGuiString& xmlfile)
    {
        CeGuiString prefix = "";
        return loadWindow(xmlfile, prefix);
    }

    CEGUI::Window* AbstractWindow::loadWindow(const CeGuiString& xmlfile, CeGuiString& prefix)
    {
        CeGuiString namePrefix;
        if (prefix == "")
            prefix.assign(StringConverter::toString(sNumAbstractWindows));
        sNumAbstractWindows++;

        CEGUI::Window* window = NULL;
        try
        {
            window = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(xmlfile, prefix);
        }
        catch (...)
        {
        }

        return window;
    }

    bool AbstractWindow::isVisible()
    {
        return mVisible;
    }

    void AbstractWindow::setVisible(bool visible, bool destroy)
    {
        if (mVisible != visible)
        {
            if (visible)
            {
                JobScheduler::getSingleton().addJob(new WindowFadeJob(this, WindowFadeJob::FADE_IN, mNormalAlpha));
            }
            else
            {
                JobScheduler::getSingleton().addJob(new WindowFadeJob(
                    this, destroy ? WindowFadeJob::FADE_OUT_AND_DESTROY : WindowFadeJob::FADE_OUT, 0.0f));
            }
            WindowManager::getSingleton()._visiblityChanged(this, visible);
            mVisible = visible;
        }
    }

    const Ogre::Real& AbstractWindow::getNormalAlpha() const
    {
        return mNormalAlpha;
    }

    bool AbstractWindow::isModal()
    {
        return mModal;
    }

    bool AbstractWindow::isClosingOnEscape()
    {
        return mCloseOnEscape;
    }

    int AbstractWindow::getWindowInputType()
    {
        return mWindowInputType;
    }

    CEGUI::Window* AbstractWindow::getRoot()
    {
        return CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
    }

    Window* AbstractWindow::getWindow(const char* name, const char* requiredClass)
    {
        CEGUI::Window* wnd = getRoot()->getChild(mNamePrefix + name);

        if (wnd == NULL)
            Throw(rl::NullPointerException, "Window " + Ogre::String(name) + " is NULL");

        if (requiredClass != NULL && wnd->getType() != requiredClass)
        {
            Throw(rl::NullPointerException,
                "Window " + Ogre::String(name) + " has not the required class " + Ogre::String(requiredClass));
        }

        return wnd;
    }

    Editbox* AbstractWindow::getEditbox(const char* name)
    {
        return static_cast<Editbox*>(getWindow(name, "Editbox"));
    }

    ToggleButton* AbstractWindow::getCheckbox(const char* name)
    {
        return static_cast<ToggleButton*>(getWindow(name, "ToggleButton"));
    }

    Listbox* AbstractWindow::getListbox(const char* name)
    {
        return static_cast<Listbox*>(getWindow(name, "Listbox"));
    }

    MultiColumnList* AbstractWindow::getMultiColumnList(const char* name)
    {
        return static_cast<MultiColumnList*>(getWindow(name, "MultiColumnList"));
    }

    MultiLineEditbox* AbstractWindow::getMultiLineEditbox(const char* name)
    {
        return static_cast<MultiLineEditbox*>(getWindow(name, "MultiLineEditbox"));
    }

    ProgressBar* AbstractWindow::getProgressBar(const char* name)
    {
        return static_cast<ProgressBar*>(getWindow(name, "ProgressBar"));
    }

    MenuBase* AbstractWindow::getMenu(const char* name)
    {
        return static_cast<MenuBase*>(getWindow(name, "MenuBase"));
    }

    MenuItem* AbstractWindow::getMenuItem(const char* name)
    {
        return static_cast<MenuItem*>(getWindow(name, "MenuItem"));
    }

    PushButton* AbstractWindow::getPushButton(const char* name)
    {
        return static_cast<PushButton*>(getWindow(name, "PushButton"));
    }

    RadioButton* AbstractWindow::getRadioButton(const char* name)
    {
        return static_cast<RadioButton*>(getWindow(name, "RadioButton"));
    }

    Combobox* AbstractWindow::getCombobox(const char* name)
    {
        return static_cast<Combobox*>(getWindow(name, "Combobox"));
    }

    ComboDropList* AbstractWindow::getComboDropList(const char* name)
    {
        return static_cast<ComboDropList*>(getWindow(name, "ComboDropList"));
    }

    ScrollablePane* AbstractWindow::getScrollablePane(const char* name)
    {
        return static_cast<ScrollablePane*>(getWindow(name, "ScrollablePane"));
    }

    Slider* AbstractWindow::getSlider(const char* name)
    {
        return static_cast<Slider*>(getWindow(name, "Slider"));
    }

    TabControl* AbstractWindow::getTabControl(const char* name)
    {
        return static_cast<TabControl*>(getWindow(name, "TabControl"));
    }

    const CeGuiString& AbstractWindow::getName() const
    {
        return mName;
    }

    void AbstractWindow::centerWindow()
    {
        auto screenSize = System::getSingleton().getRenderer()->getDisplaySize();
        auto windowSize = mWindow->getPixelSize();
        float x = 0.5f * (screenSize.d_width - windowSize.d_width);
        float y = 0.5f * (screenSize.d_height - windowSize.d_height);
        mWindow->setPosition(CeGuiHelper::asAbsolute(x, y));
    }

    void AbstractWindow::bindDestroyWindowToClick(CEGUI::Window* button)
    {
        button->subscribeEvent(Window::EventMouseClick, boost::bind(&AbstractWindow::destroyWindow, this));
    }

    void AbstractWindow::bindHideWindowToClick(CEGUI::Window* button)
    {
        button->subscribeEvent(Window::EventMouseClick, boost::bind(&AbstractWindow::hideWindow, this));
    }

    void AbstractWindow::bindDestroyWindowToXButton()
    {
        mWindow->subscribeEvent(
            CEGUI::FrameWindow::EventCloseClicked, boost::bind(&AbstractWindow::destroyWindow, this));
    }

    void AbstractWindow::bindHideWindowToXButton()
    {
        mWindow->subscribeEvent(FrameWindow::EventCloseClicked, boost::bind(&AbstractWindow::hideWindow, this));
    }

    bool AbstractWindow::destroyWindow()
    {
        setVisible(false, true);
        return true;
    }

    bool AbstractWindow::hideWindow()
    {
        setVisible(false);
        return true;
    }

    CEGUI::Window* AbstractWindow::getWindow()
    {
        return mWindow;
    }

    const CeGuiString& AbstractWindow::getNamePrefix() const
    {
        return mNamePrefix;
    }

    void AbstractWindow::windowHid()
    {
    }
}
