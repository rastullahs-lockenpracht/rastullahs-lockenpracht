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

class SaveLoadGameAction < Action
  def initialize
    super("act_saveloadgame", "Das Spiel speichern/laden")
  end

  def doAction(object, actor, target)
    $WF.showGameSaveLoadWindow()
  end
end

class ForceQuitGameAction < Action
  def initialize
    super("forcequitgame", "Das Spiel ohne Nachfrage beenden")
  end

  def doAction(object, actor, target)
    GameLoop.getSingleton().quitGame();
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
    @isShown = (not @isShown)
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

class ToggleCharacterSelectionWindowAction < Action
    def initialize
        super("togglecharacterselectionwindow", "Partyauswahl zeigen/verstecken")
    end

    def doAction(object, actor, target)
        $WF.toggleCharacterSelectionWindow()
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
    $WF.showGameObjectPropsWindow(object)
  end
  
  def canDo(object, actor, target)
    $CORE.getDeveloperMode()
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

class ShowGameSettingsAction < Action
  def initialize
    super("showgamesettings", "Spieleinstellungen")
  end

  def doAction(object, actor, target)
    $WF.showGameSettings()
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

class ShowContainerContentAction < Action
  def initialize
    super("showcontainercontent", "Inhalt anzeigen")
  end
  
  def doAction(object, actor, target)
    $WF.showContainerContent(object)
  end
  
  def canDo(object, actor, target)
    return object.isContainer()
  end
end

class InventoryShowWorldObjects < Action
  def initialize
   super("inventoryshowworldobjects", "Zeige alle greifbaren Objekte")
  end

  def doAction(object, actor, target)
  end
end

act = ShowObjectDescriptionWindow.new()
ActionManager.getSingleton().registerAction(act)

act2 = ShowGameObjectWindowAction.new()
ActionManager.getSingleton().registerAction(act2)

act3 = ShowContainerContentAction.new()
ActionManager.getSingleton().registerAction(act3)

act4 = InventoryShowWorldObjects.new()
ActionManager.getSingleton().registerAction(act4)
