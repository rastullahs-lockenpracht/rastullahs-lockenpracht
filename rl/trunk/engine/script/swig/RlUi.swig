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

namespace rl {

%apply SWIGTYPE *DYNAMIC { rl::ControlState * };

%feature("director") ControlState;
class ControlState
{
private:
    ControlState(rl::Actor* camera, rl::Actor* character);
public:
};

class CutsceneControlState : public ControlState
{
private:
       CutsceneControlState(rl::Actor* camera);

public:
    virtual ~CutsceneControlState();

    void setCameraPosition(const Ogre::Vector3& pos);
    void setCameraOrientation(const Ogre::Quaternion& orient);
    void lookAt(const Ogre::Vector3& point);
};

class UiSubsystem
{
public:
    static UiSubsystem& getSingleton(void);
    void initializeSubsystem();

    rl::Person* getActiveCharacter();
    void setActiveCharacter(rl::Person* chara);
};

enum ControlStateType 
{
    CST_NONE = 1,
    CST_FREEFLIGHT,
    CST_MOVEMENT,
    CST_DIALOG,
    CST_CUTSCENE,
    CST_COMBAT,
};

class InputManager
{
public:
    static InputManager& getSingleton(void);

    void pushControlState(rl::ControlStateType);
    void popControlState();

    void linkKeyToRubyCommand(const rl::CeGuiString &key, const rl::CeGuiString &command);
private:
    InputManager();
};

class WindowFactory
{
public:
    enum PopupType
    {
        ICON_ERROR = 1,
        ICON_QUEST
    };

    static rl::WindowFactory& getSingleton();

    void update();

    void showAboutWindow();
    void showActionChoice(rl::GameObject* obj);
    void showCharacterActionChoice();
    void showCharacterSheet();
    void showCharacterSheet(rl::Person* chara);
    void showContainerContent(rl::Container* container);
    void showDescriptionWindow(rl::GameObject* obj);
    void showPropertiesWindow(rl::GameObject* obj);
    void showGameSaveLoadWindow();
    void showExitConfirmation();
    void showJournalWindow();
    void showLogfiles();
    void showMainMenu();
    void showMessageWindow(const rl::CeGuiString& message);
    void showPlaylist();
    void showPopupMessage(int popupTypes);
    void showGameSettings();

    void toggleConsole();
    void toggleDebugWindow();
    void toggleGameLogWindow();
    void toggleCharacterStateWindow();
    void toggleInGameGlobalMenu();
    void toggleInventoryWindow();

    void checkForErrors();
    rl::GameLoggerWindow* getGameLogger();

    void showNextDebugWindowPage();
};

class WindowManager
{
public:
    static rl::WindowManager& getSingleton();
    void closeTopWindow();
};

} //namespace rl

%{
static swig_type_info *ControlState_dynamic_cast(void **ptr)
{
    rl::ControlState *pControlState =
        static_cast<rl::ControlState*>(*ptr);

    rl::CutsceneControlState *pCutSceneCS =
        dynamic_cast<rl::CutsceneControlState*>(pControlState);
    if (pCutSceneCS != NULL) {
        *ptr=pCutSceneCS;
        return SWIGTYPE_p_rl__CutsceneControlState;
    }
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__ControlState, ControlState_dynamic_cast);
