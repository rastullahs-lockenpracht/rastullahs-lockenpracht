load 'embed.rb'
load 'global-settings.rb'

GlobalSettings.addIngameActions(ActionManager.getSingleton())
WindowFactory.getSingleton().update()
