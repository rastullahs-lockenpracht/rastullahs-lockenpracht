#require 'RlScript'

load 'actions.rb'

class ShowActionMenuAction < RubyAction
  def initialize
    super("Aktionen", "Aktionen anzeigen")
  end
  
  def doAction(object, actor, target)
    $UI.showCharacterActionChoice
  end
end

class CampAction < RubyAction
  def initialize
    super("Rasten", "Das Zelt aufschlagen und rasten")
  end
  
  def doAction(object, actor, target)
    $UI.showMessageWindow("Hier kannst du nicht schlafen")
  end
end

class QuitGameAction < RubyAction
  def initialize
    super("Ende", "Das Spiel beenden")
  end
  
  def doAction(object, actor, target)
    $UI.requestExit()
  end
end

class ToggleConsoleAction < RubyAction
  def initialize
    super("Konsole", "Konsole zeigen/verstecken")
  end
  
  def doAction(object, actor, target)
    $UI.toggleConsole()
  end
end

class ToggleDebugWindowAction < RubyAction
  def initialize
    super("DebugWindow", "DebugWindow zeigen/verstecken")
  end
  
  def doAction(object, actor, target)
    $UI.toggleDebugWindow()
  end
end

class ToggleGameLogWindowAction < RubyAction
    def initialize
        super("GameLogWindow", "Game Log zeigen/verstecken")
    end
    
    def doAction(object, actor, target)
        $UI.toggleGameLogWindow()
    end
end

class ToggleViewModeAction < RubyAction
  def initialize
    super("Sichtmodus", "First/ThirdPerson umschalten")
  end
  
  def doAction(object, actor, target)
    gc = $UI.getGameController();
    gc.setViewMode((gc.getViewMode() + 1) % 2);
  end
end

class ResetCameraAction < RubyAction
  def initialize
    super("Kamera", "Kamera zurücksetzen")
  end
  
  def doAction(object, actor, target)
    gc = $UI.getGameController();
    gc.resetCamera();
  end
end

class MakeScreenshotAction < RubyAction
  def initialize
    super("Screenshot", "Screenshot erstellen")
  end
  
  def doAction(object, actor, target)
    $CORE.makeScreenshot("rastullah");
  end
end

class ToggleOdeDebugAction < RubyAction
  def initialize
    super("OdeDebug", "Ode-Koerper anzeigen")
  end
  
  def doAction(object, actor, target)
    gc = PhysicsManager.getSingleton
    gc.toggleDebugOde();
  end
end

class ShowObjectActionsAction < RubyAction
  def initialize
    super("ShowObjectActions", "Aktionen anzeigen")
  end

  def doAction(object, actor, target)
    $UI.showPickedObjectActions()
  end
end

class ShowCharacterSheetAction < RubyAction
  def initialize
    super("Charakterblatt", "Charakterblatt des aktiven Helden anzeigen")
  end

  def doAction(object, actor, target)
    $UI.showCharacterSheet()
  end
end


class PlayerSettings
  
  def PlayerSettings.addPlayerActions(player)    
    grpGrp1 = ActionGroup.new("Spielaktionen")
    grpGrp2 = ActionGroup.new("Anzeige")
    grpGrp3 = ActionGroup.new("Optionen")
    
    player.addActionInGroup(ShowActionMenuAction.new, grpGrp1, 7) # ACT_DISABLED
    player.addActionInGroup(CampAction.new, grpGrp1) 
    player.addActionInGroup(ShowObjectActionsAction.new, grpGrp1) 
    player.addActionInGroup(ToggleConsoleAction.new, grpGrp2)
    player.addActionInGroup(ToggleDebugWindowAction.new, grpGrp2)
    player.addActionInGroup(ToggleGameLogWindowAction.new, grpGrp2)
    player.addActionInGroup(ToggleOdeDebugAction.new, grpGrp2)
    player.addActionInGroup(ToggleViewModeAction.new, grpGrp2)
    player.addActionInGroup(ResetCameraAction.new, grpGrp2)
    player.addActionInGroup(ShowCharacterSheetAction.new, grpGrp2)
    player.addActionInGroup(QuitGameAction.new, grpGrp3) 
    player.addActionInGroup(MakeScreenshotAction.new, grpGrp3)
  end
  
  def PlayerSettings.preparePlayer(player)
    PlayerSettings.addPlayerActions(player)        # mal gucken, was noch so kommt
  end
  
end
