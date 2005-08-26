load 'embed.rb'
load 'actions.rb'

class ShowActionMenuAction < RubyAction
  def initialize
    super("showactions", "Aktionen anzeigen")
  end

  def doAction(object, actor, target)
    $UI.showCharacterActionChoice
  end
end

class QuitGameAction < RubyAction
  def initialize
    super("quitgame", "Das Spiel beenden")
  end

  def doAction(object, actor, target)
    $UI.requestExit()
  end
end

class ToggleConsoleAction < RubyAction
  def initialize
    super("toggleconsole", "Konsole zeigen/verstecken")
  end
  
  def doAction(object, actor, target)
    $UI.toggleConsole()
  end
end

class ToggleDebugWindowAction < RubyAction
  def initialize
    super("toggledebugwindow", "DebugWindow zeigen/verstecken")
  end

  def doAction(object, actor, target)
    $UI.toggleDebugWindow()
  end
end

class ToggleGameLogWindowAction < RubyAction
    def initialize
        super("togglegamelogwindow", "Game Log zeigen/verstecken")
    end

    def doAction(object, actor, target)
        $UI.toggleGameLogWindow()
    end
end

class ToggleViewModeAction < RubyAction
  def initialize
    super("toggleviewmode", "First/ThirdPerson umschalten")
  end

  def doAction(object, actor, target)
    gc = $UI.getGameController();
    gc.toggleViewMode()
  end
end

class ToggleInGameGlobalMenuAction < RubyAction
  def initialize
    super("toggleingameglobalmenu", "Spielmenü anzeigen")
  end

  def doAction(object, actor, target)
    $UI.toggleInGameGlobalMenu()
  end
end

class ResetCameraAction < RubyAction
  def initialize
    super("resetcamera", "Kamera zurücksetzen")
  end

  def doAction(object, actor, target)
    gc = $UI.getGameController();
    gc.resetCamera();
  end
end

class MakeScreenshotAction < RubyAction
  def initialize
    super("makescreenshot", "Screenshot erstellen")
  end
  
  def doAction(object, actor, target)
    $CORE.makeScreenshot("rastullah");
  end
end

class ToggleOdeDebugAction < RubyAction
  def initialize
    super("toggleodedebug", "Ode-Körper anzeigen")
  end
  
  def doAction(object, actor, target)
    $PM.toggleDebugOde();
  end
end

class ToggleCharacterStateWindowAction < RubyAction
    def initialize
        super("togglecharacterstatewindow", "Charakterstatus zeigen/verstecken")
    end

    def doAction(object, actor, target)
        $UI.toggleCharacterStateWindow()
    end
end

class ShowObjectActionsAction < RubyAction
  def initialize
    super("showobjectactions", "Objektaktionen anzeigen")
  end

  def doAction(object, actor, target)
    $UI.showPickedObjectActions()
  end
end

class ShowCharacterSheetAction < RubyAction
  def initialize
    super("showcharactersheet", "Charakterblatt des aktiven Helden anzeigen")
  end

  def doAction(object, actor, target)
    $UI.showCharacterSheet()
  end
end

class ToggleDialogWindowAction < RubyAction
  def initialize
    super("toggledialogwindow", "Charakterblatt des aktiven Helden anzeigen")
  end

  def doAction(object, actor, target)
    $UI.toggleDialogWindow()
  end
end

class ShowTargetWindowAction < RubyAction
  def initialize
    super("showtargetwindow", "Zielauswahl")
  end

  def doAction(object, actor, target)
    $UI.showTargetWindow()
  end
end

class UseGameObjectDefaultAction < RubyAction
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

act = ShowObjectDescriptionWindow.new()
ActionManager.getSingleton().registerAction(act)