load 'embed.rb'
load 'global-settings.rb'

GlobalSettings.addIngameActions(ActionManager.getSingleton())
WindowManager.getSingleton().update()
