load 'embed.rb'

class ShowActionMenuAction < Action
  def initialize
    super("act_showactions", "Aktionen anzeigen")
  end

  def doAction(object, actor, target)
    $WF.showCharacterActionChoice()
  end
end

class QuitGameAction < Action
  def initialize
    super("act_quitgame", "Das Spiel beenden")
  end

  def doAction(object, actor, target)
    $WF.showExitConfirmation()
  end
end

class ForceQuitGameAction < Action
  def initialize
    super("act_forcequitgame", "Das Spiel ohne Nachfrage beenden")
  end

  def doAction(object, actor, target)
    GameLoop.getSingleton().quitGame();
  end
end

class ToggleConsoleAction < Action
  def initialize
    super("act_toggleconsole", "Konsole zeigen/verstecken")
  end

  def doAction(object, actor, target)
    $WF.toggleConsole()
  end
end

class ToggleDebugWindowAction < Action
  def initialize
    super("act_toggledebugwindow", "DebugWindow zeigen/verstecken")
  end

  def doAction(object, actor, target)
    $WF.toggleDebugWindow()
  end
end

class ToggleGameLogWindowAction < Action
    def initialize
        super("act_togglegamelogwindow", "Game Log zeigen/verstecken")
    end

    def doAction(object, actor, target)
        $WF.toggleGameLogWindow()
    end
end

class ToggleViewModeAction < Action
  def initialize
    super("act_toggleviewmode", "First/ThirdPerson umschalten")
  end

  def doAction(object, actor, target)
    gc = $UI.getCharacterController();
    gc.toggleViewMode()
  end
end

class ToggleInGameGlobalMenuAction < Action
  def initialize
    super("act_toggleingameglobalmenu", "Spielmenü anzeigen")
  end

  def doAction(object, actor, target)
    $WF.toggleInGameGlobalMenu()
  end
end

class ToggleInventoryAction < Action
	def initialize
		super("act_toggleinventorywindow", "Das Inventar des aktiven Helden anzeigen")
	end

	def doAction(object, actor, target)
		$WF.toggleInventoryWindow()
	end
end

class ResetCameraAction < Action
  def initialize
    super("act_resetcamera", "Kamera zurücksetzen")
  end

  def doAction(object, actor, target)
    gc = $UI.getCharacterController();
    gc.resetCamera();
  end
end

class MakeScreenshotAction < Action
  def initialize
    super("act_makescreenshot", "Screenshot erstellen")
  end

  def doAction(object, actor, target)
    $CORE.makeScreenshot("rastullah");
  end
end

class ToggleCollisionDebugAction < Action
  def initialize
    super("act_togglecollisiondebug", "Kollisionskörper anzeigen")
  end

  def doAction(object, actor, target)
    $PM.toggleDebugMode();
  end
end

class ToggleDebugVisualisationAction < Action
  def initialize
    super("act_toggledebugvisualisation", "Debug-Helfer anzeigen")
    @isShown = false
  end

  def doAction(object, actor, target)
    if @isShown:
      $DVM.hideAll()
    else
      $DVM.showAll()
    end
    @isShown = (not @isShown)
  end
end

class ToggleCharacterStateWindowAction < Action
    def initialize
        super("act_togglecharacterstatewindow", "Charakterstatus zeigen/verstecken")
    end

    def doAction(object, actor, target)
        $WF.toggleCharacterStateWindow()
    end
end

class ShowCharacterSheetAction < Action
  def initialize
    super("act_showcharactersheet", "Charakterblatt des aktiven Helden anzeigen")
  end

  def doAction(object, actor, target)
    $WF.showCharacterSheet()
  end
end

class ShowTargetWindowAction < Action
  def initialize
    super("act_showtargetwindow", "Zielauswahl")
  end

  def doAction(object, actor, target)
    $WF.showTargetWindow()
  end
end

class ShowJournalWindowAction < Action
  def initialize
    super("act_showjournalwindow", "Tagebuch")
  end

  def doAction(object, actor, target)
    $WF.showJournalWindow()
  end
end

class ShowObjectDescriptionWindow < Action
  def initialize
    super(GameObject.DEFAULT_VIEW_OBJECT_ACTION, "Anschauen")
  end

  def doAction(object, actor, target)
    $WF.showDescriptionWindow(object)
  end
end

class ShowGameObjectWindowAction < Action
  def initialize
    super(GameObject.DEFAULT_VIEW_OBJECT_ACTION_DEBUG, "GameObject Properties (DeveloperMode)")
  end
  
  def doAction(object, actor, target)
    $WF.showGameObjectWindow(object)
  end
  
  def canDo(object, actor)
    $CORE.getDeveloperMode()
  end
end

class ShowAboutWindowAction < Action
  def initialize
    super("act_showaboutwindow", "Über")
  end

  def doAction(object, actor, target)
    $WF.showAboutWindow()
  end
end

class ShowGameSettingsAction < Action
  def initialize
    super("act_showgamesettings", "Spieleinstellungen")
  end

  def doAction(object, actor, target)
    $WF.showGameSettings()
  end
end

class ShowLogfilesAction < Action
  def initialize
    super("act_showlogfiles", "Logfiles anzeigen")
  end

  def doAction(object, actor, target)
    $WF.showLogfiles()
  end
end

class ShowNextDebugWindowPageAction < Action
  def initialize
    super("act_shownextdebugwindowpage", "Zur nächsten Info-Seite wechseln")
  end

  def doAction(object, actor, target)
    $WF.showNextDebugWindowPage()
  end
end

class CloseCurrentWindowAction < Action
  def initialize
    super("act_closecurrentwindow", "Oberstes Fenster schliessen")
  end

  def doAction(object, actor, target)
    WindowManager.getSingleton().closeTopWindow()
  end
end

class ToggleCharacterControllerAction < Action
  def initialize
    super("act_togglecharactercontroller",
        "Freeflight- / Movement-Controller umschalten")
  end

  def doAction(object, actor, target)
    cctype = $UI.getCharacterControllerType();
    if (cctype == CharacterController::CTRL_FREEFLIGHT)
        $UI.setCharacterController(CharacterController::CTRL_MOVEMENT);
    elsif (cctype == CharacterController::CTRL_MOVEMENT)
        $UI.setCharacterController(CharacterController::CTRL_FREEFLIGHT);
    end
  end
end

class ShowContainerContentAction < Action
  def initialize
    super("act_showcontainercontent", "Inhalt anzeigen")
  end
  
  def doAction(object, actor, target)
    $WF.showContainerContent(object)
  end
  
  def canDo(object, actor)
    return object.isContainer()
  end
end


act = ShowObjectDescriptionWindow.new()
ActionManager.getSingleton().registerAction(act)

act2 = ShowGameObjectWindowAction.new()
ActionManager.getSingleton().registerAction(act2)

act3 = ShowContainerContentAction.new()
ActionManager.getSingleton().registerAction(act3)
