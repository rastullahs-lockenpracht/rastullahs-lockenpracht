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
        player.addAction(ShowActionMenuAction.new, 7) # ACT_DISABLED
        player.addAction(CampAction.new) 
        player.addAction(QuitGameAction.new) 
        player.addAction(ToggleConsoleAction.new)
        player.addAction(ToggleDebugWindowAction.new)
        player.addAction(ToggleViewModeAction.new)
        player.addAction(ResetCameraAction.new)
        player.addAction(MakeScreenshotAction.new)
    end

    def PlayerSettings.preparePlayer(player)
        PlayerSettings.addPlayerActions(player)        # mal gucken, was noch so kommt
    end
    
end
