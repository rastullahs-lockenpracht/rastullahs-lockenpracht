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


class PlayerSettings

	def PlayerSettings.addPlayerActions(player)
		player.addAction(ShowActionMenuAction.new, 4) # ACT_DISABLED
		player.addAction(CampAction.new) 
		player.addAction(QuitGameAction.new) 
		player.addAction(ToggleConsoleAction.new)
		player.addAction(ToggleDebugWindowAction.new)
	end

	def PlayerSettings.preparePlayer(player)
		PlayerSettings.addPlayerActions(player)		# mal gucken, was noch so kommt
	end
	
end
