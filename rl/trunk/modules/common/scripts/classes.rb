load "embed.rb"

include RlScript

class RastullahCommands < CommandExecutor

  public
	  def getCommandsInBattle
		  return getCommandsStartWith("cmdB_")
	  end
	
    def getCommandsOffBattle
		  return getCommandsStartWith("cmdP_")
	  end
	
    def getCommandsStartWith(start)
	  	commands = Array.new
 		  methods = self.class.private_instance_methods - self.class.superclass.private_instance_methods
 		  methods.each { |method| commands.push(method) if method[0, start.length] == start }
		  return commands
	  end
	
    def executeCommand(command)
		  send(command)
	  end
	
	# und hier die Methoden hin, die aufgerufen werden sollen
  private
  
    def cmdB_Angriff
	  end
	
    def cmdP_Singen
	  end
	
end
