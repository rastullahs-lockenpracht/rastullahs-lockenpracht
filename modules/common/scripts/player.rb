#require 'RlScript'

load 'actions.rb'

class ShowActionMenuAction < RubyAction
  def initialize
    super("showactions", "Aktionen anzeigen")
  end
  
  def doAction(object, actor, target)
    $UI.showCharacterActionChoice
  end
end

class CampAction < RubyAction
  def initialize
    super("makecamp", "Das Zelt aufschlagen und rasten")
  end
  
  def doAction(object, actor, target)
    $UI.showMessageWindow("Hier kannst du nicht schlafen")
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
    gc.setViewMode((gc.getViewMode() + 1) % 2);
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
    super("toggleodedebug", "Ode-Koerper anzeigen")
  end
  
  def doAction(object, actor, target)
    $PM.toggleDebugOde();
  end
end

class ToggleCharacterStateWindowAction < RubyAction
    def initialize
        super("togglecharacterstatewindow", "Characterstatus zeigen/verstecken")
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


class PlayerSettings
  
  def PlayerSettings.addPlayerActions(player)    
    $grpGrp1 = ActionGroup.new("GameActions")
    $grpGrp2 = ActionGroup.new("View")
    $grpGrp3 = ActionGroup.new("Options")

    $act1 = ShowActionMenuAction.new
    $act2 = CampAction.new
    $act3 = ShowObjectActionsAction.new
    $act4 = ToggleConsoleAction.new
    $act5 = ToggleDebugWindowAction.new
    $act6 = ToggleGameLogWindowAction.new
    $act7 = ToggleOdeDebugAction.new
    $act8 = ToggleDialogWindowAction.new
    $act9 = ToggleViewModeAction.new
    $act10 = ResetCameraAction.new
    $act11 = ShowCharacterSheetAction.new
    $act12 = QuitGameAction.new
    $act13 = MakeScreenshotAction.new
    $act14 = ToggleCharacterStateWindowAction.new
    $act15 = ShowTargetWindowAction.new

    player.addActionInGroup($act1, $grpGrp1, 7) # ACT_DISABLED
    player.addActionInGroup($act2, $grpGrp1)
    player.addActionInGroup($act3, $grpGrp1)

    player.addActionInGroup($act4, $grpGrp2)
    player.addActionInGroup($act5, $grpGrp2)
    player.addActionInGroup($act6, $grpGrp2)
    player.addActionInGroup($act7, $grpGrp2)
    player.addActionInGroup($act8, $grpGrp2)
    player.addActionInGroup($act9, $grpGrp2)
    player.addActionInGroup($act10, $grpGrp2)
    player.addActionInGroup($act11, $grpGrp2)
    player.addActionInGroup($act14, $grpGrp2)
    player.addActionInGroup($act15, $grpGrp2)

    player.addActionInGroup($act12, $grpGrp3)
    player.addActionInGroup($act13, $grpGrp3)
    
  end
  
  def PlayerSettings.preparePlayer(player)
    PlayerSettings.addPlayerActions(player)        # mal gucken, was noch so kommt
  end
  
end
