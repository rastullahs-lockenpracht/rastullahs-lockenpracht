load 'embed.rb'

class ShowActionMenuAction < Action
  def initialize
    super("showactions", "Aktionen anzeigen")
  end

  def doAction(object, actor, target)
    $UI.showCharacterActionChoice
  end
end

class QuitGameAction < Action
  def initialize
    super("quitgame", "Das Spiel beenden")
  end

  def doAction(object, actor, target)
    $UI.requestExit()
  end
end

class ToggleConsoleAction < Action
  def initialize
    super("toggleconsole", "Konsole zeigen/verstecken")
  end
  
  def doAction(object, actor, target)
    $UI.toggleConsole()
  end
end

class ToggleDebugWindowAction < Action
  def initialize
    super("toggledebugwindow", "DebugWindow zeigen/verstecken")
  end

  def doAction(object, actor, target)
    $UI.toggleDebugWindow()
  end
end

class ToggleGameLogWindowAction < Action
    def initialize
        super("togglegamelogwindow", "Game Log zeigen/verstecken")
    end

    def doAction(object, actor, target)
        $UI.toggleGameLogWindow()
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
    $UI.toggleInGameGlobalMenu()
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

class ToggleCharacterStateWindowAction < Action
    def initialize
        super("togglecharacterstatewindow", "Charakterstatus zeigen/verstecken")
    end

    def doAction(object, actor, target)
        $UI.toggleCharacterStateWindow()
    end
end

class ShowObjectActionsAction < Action
  def initialize
    super("showobjectactions", "Objektaktionen anzeigen")
  end

  def doAction(object, actor, target)
    $UI.showPickedObjectActions()
  end
end

class ShowCharacterSheetAction < Action
  def initialize
    super("showcharactersheet", "Charakterblatt des aktiven Helden anzeigen")
  end

  def doAction(object, actor, target)
    $UI.showCharacterSheet()
  end
end

class ToggleDialogWindowAction < Action
  def initialize
    super("toggledialogwindow", "Charakterblatt des aktiven Helden anzeigen")
  end

  def doAction(object, actor, target)
    $UI.toggleDialogWindow()
  end
end

class ShowTargetWindowAction < Action
  def initialize
    super("showtargetwindow", "Zielauswahl")
  end

  def doAction(object, actor, target)
    $UI.showTargetWindow()
  end
end

class ShowJournalWindowAction < Action
  def initialize
    super("showjournalwindow", "Tagebuch")
  end

  def doAction(object, actor, target)
    $UI.showJournalWindow()
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
    $UI.showDescriptionWindow(object)
  end
end

class ShowTestVideoWindow < Action
  def initialize
    super("showtestvideowindow", "Video")
  end
  
  def doAction(object, actor, target)
    load "video.rb"
  end
end

class ShowAboutWindowAction < Action
  def initialize
    super("showaboutwindow", "Über")
  end
  
  def doAction(object, actor, target)
    $UI.showAboutWindow()
  end
end

act = ShowObjectDescriptionWindow.new()
ActionManager.getSingleton().registerAction(act)
