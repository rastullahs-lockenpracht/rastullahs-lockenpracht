load 'embed.rb'
load 'global-actions.rb'

class GlobalSettings
  def GlobalSettings.addIngameActions(actionManager)
      $grpView = ActionGroup.new("View")
      $grpOptions = ActionGroup.new("Options")

      $act1 = ShowActionMenuAction.new
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
      $act16 = ToggleInGameGlobalMenuAction.new

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

      actionManager.registerInGameGlobalAction($act12, $grpOptions)
      actionManager.registerInGameGlobalAction($act13, $grpOptions)

  end
end