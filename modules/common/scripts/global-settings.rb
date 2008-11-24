load 'embed.rb'
load 'global-actions.rb'

class GlobalSettings
  def GlobalSettings.addIngameActions(actionManager)
      $grpView = ActionGroup.new("View")
      $grpActions = ActionGroup.new("Aktionen")
      $grpHelp = ActionGroup.new("Hilfe")
      $grpOptions = ActionGroup.new("Optionen")

      actionManager.registerInGameGlobalAction(ShowActionMenuAction.new(), $grpView)
      actionManager.registerInGameGlobalAction(ToggleConsoleAction.new(), $grpView)
      actionManager.registerInGameGlobalAction(ToggleDebugWindowAction.new(), $grpView)
      actionManager.registerInGameGlobalAction(ToggleGameLogWindowAction.new(), $grpView)
      actionManager.registerInGameGlobalAction(ToggleCollisionDebugAction.new(), $grpView)
      actionManager.registerInGameGlobalAction(ShowCharacterSheetAction.new(), $grpView)
      actionManager.registerInGameGlobalAction(ToggleCharacterStateWindowAction.new(), $grpView)
      actionManager.registerInGameGlobalAction(ToggleCharacterSelectionWindowAction.new(), $grpView)
      actionManager.registerInGameGlobalAction(ShowTargetWindowAction.new(), $grpView)
      actionManager.registerInGameGlobalAction(ToggleInGameGlobalMenuAction.new(), $grpView)
      actionManager.registerInGameGlobalAction(ShowJournalWindowAction.new(), $grpView)
      actionManager.registerInGameGlobalAction(CloseCurrentWindowAction.new(), $grpView)
      actionManager.registerInGameGlobalAction(ShowNextDebugWindowPageAction.new(), $grpView)
      actionManager.registerInGameGlobalAction(ToggleInventoryAction.new(), $grpView)
      actionManager.registerInGameGlobalAction(ToggleDebugVisualisationAction.new(), $grpView)
      actionManager.registerInGameGlobalAction(ForceQuitGameAction.new(), $grpView)

      actionManager.registerInGameGlobalAction(QuitGameAction.new(), $grpOptions)
      actionManager.registerInGameGlobalAction(MakeScreenshotAction.new(), $grpOptions)
      actionManager.registerInGameGlobalAction(ShowGameSettingsAction.new(), $grpOptions)
      actionManager.registerInGameGlobalAction(ShowLogfilesAction.new(), $grpOptions)
      actionManager.registerInGameGlobalAction(SaveLoadGameAction.new(), $grpOptions)

      actionManager.registerInGameGlobalAction(ShowAboutWindowAction.new(), $grpHelp)
  end
end
