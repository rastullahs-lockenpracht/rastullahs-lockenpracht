load "embed.rb"

include RlScript

class ScriptTask < TimerTask

	def initialize(name)
		super(name)
	end
	
	def run(name)
	  send(name)
	end

end