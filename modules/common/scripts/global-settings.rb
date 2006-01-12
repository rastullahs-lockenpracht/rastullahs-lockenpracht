load 'embed.rb'
load 'global-actions.rb'

class GlobalSettings
  def GlobalSettings.addIngameActions(actionManager)
      $grpView = ActionGroup.new("View")
      $grpActions = ActionGroup.new("Aktionen")
      $grpHelp = ActionGroup.new("Hilfe")
      $grpOptions = ActionGroup.new("Optionen")

      $act1 = ShowActionMenuAction.new
      $act3 = ShowObjectActionsAction.new
      $act4 = ToggleConsoleAction.new
      $act5 = ToggleDebugWindowAction.new
      $act6 = ToggleGameLogWindowAction.new
      $act7 = ToggleCollisionDebugAction.new
      $act8 = ToggleDialogWindowAction.new
      $act9 = ToggleViewModeAction.new
      $act10 = ResetCameraAction.new
      $act11 = ShowCharacterSheetAction.new
      $act12 = QuitGameAction.new
      $act13 = MakeScreenshotAction.new
      $act14 = ToggleCharacterStateWindowAction.new
      $act15 = ShowTargetWindowAction.new
      $act16 = ToggleInGameGlobalMenuAction.new
      $act17 = ShowTestVideoWindow.new
      $act18 = UseGameObjectDefaultAction.new
      $act19 = ShowAboutWindowAction.new
      $act20 = ShowJournalWindowAction.new
      $act21 = CloseCurrentWindowAction.new

      actionManager.registerInGameGlobalAction($act1, $grpView)
      actionManager.registerInGameGlobalAction($act3, $grpView)
      actionManager.registerInGameGlobalAction($act4, $grpView)
      actionManager.registerInGameGlobalAction($act5, $grpView)
      actionManager.registerInGameGlobalAction($act6, $grpView)
      actionManager.registerInGameGlobalAction($act7, $grpView)
      actionManager.registerInGameGlobalAction($act8, $grpView)
      actionManager.registerInGameGlobalAction($act9, $grpView)
      actionManager.registerInGameGlobalAction($act10, $grpView)
      actionManager.registerInGameGlobalAction($act11, $grpView)
      actionManager.registerInGameGlobalAction($act14, $grpView)
      actionManager.registerInGameGlobalAction($act15, $grpView)
      actionManager.registerInGameGlobalAction($act16, $grpView)
      actionManager.registerInGameGlobalAction($act17, $grpView)
      actionManager.registerInGameGlobalAction($act20, $grpView)
      actionManager.registerInGameGlobalAction($act21, $grpView)

      actionManager.registerInGameGlobalAction($act12, $grpOptions)
      actionManager.registerInGameGlobalAction($act13, $grpOptions)

      actionManager.registerInGameGlobalAction($act19, $grpHelp)
      actionManager.registerInGameGlobalAction($act18, $grpActions)


  end
end
