#require 'RlScript'

load 'actions.rb'

class ShowActionMenuAction < RubyAction
    def initialize
        super("Aktionen", "Aktionen anzeigen")
    end
    
    def doAction(object, actor, target)
        $Ui.showCharacterActionChoice
    end
end

class CampAction < RubyAction
    def initialize
        super("Rasten", "Das Zelt aufschlagen und rasten")
    end
    
    def doAction(object, actor, target)
        $Ui.showMessageWindow("Hier kannst du nicht schlafen")
    end
end

class QuitGameAction < RubyAction
    def initialize
        super("Ende", "Das Spiel beenden")
    end
    
    def doAction(object, actor, target)
        $Ui.requestExit()
    end
end
    
class ToggleConsoleAction < RubyAction
    def initialize
        super("Konsole", "Konsole zeigen/verstecken")
    end
    
    def doAction(object, actor, target)
        $Ui.toggleConsole()
    end
end

class ToggleDebugWindowAction < RubyAction
    def initialize
        super("DebugWindow", "DebugWindow zeigen/verstecken")
    end
    
    def doAction(object, actor, target)
        $Ui.toggleDebugWindow()
    end
end

class ToggleViewModeAction < RubyAction
    def initialize
        super("Sichtmodus", "First/ThirdPerson umschalten")
    end
    
    def doAction(object, actor, target)
        gc = $Ui.getGameController();
        gc.setViewMode((gc.getViewMode() + 1) % 2);
    end
end

class ResetCameraAction < RubyAction
    def initialize
        super("Kamera", "Kamera zurücksetzen")
    end
    
    def doAction(object, actor, target)
        gc = $Ui.getGameController();
        gc.resetCamera();
    end
end

class MakeScreenshotAction < RubyAction
    def initialize
        super("Screenshot", "Screenshot erstellen")
    end
    
    def doAction(object, actor, target)
        CoreSubsystem.getSingleton().makeScreenshot("rastullah");
    end
end

class PlayerSettings

    def PlayerSettings.addPlayerActions(player)
    
    	  grpGrp1 = ActionGroup.new("Grp1")
    	  grpGrp2 = ActionGroup.new("Grp2")
    	  grpGrp3 = ActionGroup.new("Grp3")
    	  
    
        player.addActionInGroup(ShowActionMenuAction.new, grpGrp1, 7) # ACT_DISABLED
        player.addActionInGroup(CampAction.new, grpGrp1) 
        player.addActionInGroup(ToggleConsoleAction.new, grpGrp1)
        player.addActionInGroup(ToggleDebugWindowAction.new, grpGrp2)
        player.addActionInGroup(ToggleViewModeAction.new, grpGrp2)
        player.addActionInGroup(ResetCameraAction.new, grpGrp2)
        player.addActionInGroup(QuitGameAction.new, grpGrp3) 
        player.addActionInGroup(MakeScreenshotAction.new, grpGrp3)
    end

    def PlayerSettings.preparePlayer(player)
        PlayerSettings.addPlayerActions(player)        # mal gucken, was noch so kommt
    end
    
end
