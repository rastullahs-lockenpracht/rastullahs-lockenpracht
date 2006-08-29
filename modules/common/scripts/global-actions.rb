load 'embed.rb'

class ShowActionMenuAction < Action
  def initialize
    super("showactions", "Aktionen anzeigen")
  end

  def doAction(object, actor, target)
    $WF.showCharacterActionChoice()
  end
end

class QuitGameAction < Action
  def initialize
    super("quitgame", "Das Spiel beenden")
  end

  def doAction(object, actor, target)
    $WF.showExitConfirmation()
  end
end

class ToggleConsoleAction < Action
  def initialize
    super("toggleconsole", "Konsole zeigen/verstecken")
  end
  
  def doAction(object, actor, target)
    $WF.toggleConsole()
  end
end

class ToggleDebugWindowAction < Action
  def initialize
    super("toggledebugwindow", "DebugWindow zeigen/verstecken")
  end

  def doAction(object, actor, target)
    $WF.toggleDebugWindow()
  end
end

class ToggleGameLogWindowAction < Action
    def initialize
        super("togglegamelogwindow", "Game Log zeigen/verstecken")
    end

    def doAction(object, actor, target)
        $WF.toggleGameLogWindow()
    end
end

class ToggleViewModeAction < Action
  def initialize
    super("toggleviewmode", "First/ThirdPerson umschalten")
  end

  def doAction(object, actor, target)
    gc = $UI.getCharacterController();
    gc.toggleViewMode()
  end
end

class ToggleInGameGlobalMenuAction < Action
  def initialize
    super("toggleingameglobalmenu", "Spielmenü anzeigen")
  end

  def doAction(object, actor, target)
    $WF.toggleInGameGlobalMenu()
  end
end

class ToggleInventoryAction < Action
	def initialize
		super("toggleinventorywindow", "Das Inventar des aktiven Helden anzeigen")
	end
	
	def doAction(object, actor, target)
		$WF.toggleInventoryWindow()
	end
end

class ResetCameraAction < Action
  def initialize
    super("resetcamera", "Kamera zurücksetzen")
  end

  def doAction(object, actor, target)
    gc = $UI.getCharacterController();
    gc.resetCamera();
  end
end

class MakeScreenshotAction < Action
  def initialize
    super("makescreenshot", "Screenshot erstellen")
  end
  
  def doAction(object, actor, target)
    $CORE.makeScreenshot("rastullah");
  end
end

class ToggleCollisionDebugAction < Action
  def initialize
    super("togglecollisiondebug", "Kollisionskörper anzeigen")
  end
  
  def doAction(object, actor, target)
    $PM.toggleDebugMode();
  end
end

class ToggleDebugVisualisationAction < Action
  def initialize
    super("toggledebugvisualisation", "Debug-Helfer anzeigen")
    @isShown = false
  end
  
  def doAction(object, actor, target)
    if @isShown:
      $DVM.hideAll()
    else
      $DVM.showAll()
    end
    @isShown = (@isShown == false)
  end
end

class ToggleCharacterStateWindowAction < Action
    def initialize
        super("togglecharacterstatewindow", "Charakterstatus zeigen/verstecken")
    end

    def doAction(object, actor, target)
        $WF.toggleCharacterStateWindow()
    end
end

class ShowObjectActionsAction < Action
  def initialize
    super("showobjectactions", "Objektaktionen anzeigen")
  end

  def doAction(object, actor, target)
    $WF.showPickedObjectActions()
  end
end

class ShowCharacterSheetAction < Action
  def initialize
    super("showcharactersheet", "Charakterblatt des aktiven Helden anzeigen")
  end

  def doAction(object, actor, target)
    $WF.showCharacterSheet()
  end
end

class ShowTargetWindowAction < Action
  def initialize
    super("showtargetwindow", "Zielauswahl")
  end

  def doAction(object, actor, target)
    $WF.showTargetWindow()
  end
end

class ShowJournalWindowAction < Action
  def initialize
    super("showjournalwindow", "Tagebuch")
  end

  def doAction(object, actor, target)
    $WF.showJournalWindow()
  end
end

class UseGameObjectDefaultAction < Action
  def initialize
    super("usecurrentobjectdefaultaction", "Standardaktion benutzen")
  end
  
  def doAction(object, actor, target)
    $UI.usePickedObjectDefaultActions()
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

class ShowAboutWindowAction < Action
  def initialize
    super("showaboutwindow", "Über")
  end
  
  def doAction(object, actor, target)
    $WF.showAboutWindow()
  end
end

class ShowSoundConfigAction < Action
  def initialize
    super("showsoundconfig", "Soundeinstellungen")
  end

  def doAction(object, actor, target)
    $WF.showSoundConfig()
  end
end

class ShowLogfilesAction < Action
  def initialize
    super("showlogfiles", "Logfiles anzeigen")
  end

  def doAction(object, actor, target)
    $WF.showLogfiles()
  end
end

class ShowNextDebugWindowPageAction < Action
  def initialize
    super("shownextdebugwindowpage", "Zur nächsten Info-Seite wechseln")
  end

  def doAction(object, actor, target)
    $WF.showNextDebugWindowPage()
  end
end

class CloseCurrentWindowAction < Action
  def initialize
    super("closecurrentwindow", "Oberstes Fenster schliessen")
  end
  
  def doAction(object, actor, target)
    WindowManager.getSingleton().closeTopWindow()
  end
end

class ToggleCharacterControllerAction < Action
  def initialize
    super("togglecharactercontroller",
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

act = ShowObjectDescriptionWindow.new()
ActionManager.getSingleton().registerAction(act)
