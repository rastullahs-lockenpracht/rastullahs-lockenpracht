load "embed.rb"

class GameActorScriptOne 
	def initialize( name ) 
		@name = name;
	end
	
	def OnPlaySound()
		print( "So was, "+@name+" hat gebeept. " );
	end 
	
	def TestReturner()
		print( "Blupp" )
		return 4
	end

	def Multiply(n,m)
		 return n.to_i() * m.to_i()
	end
end

class DialogScriptObjectTest < DialogScriptObject
	def initialize()
		super()
		@name = "AlrikTest"
		print(@name + " entschließt sich dazu, redselig zu werden") 
	end

	def calcOptionValue( optionName )
		print("Calc Option Values")
		return 4
	end

	def calcResponseValue( responseName )
		print("Calc Response Value")
		return 4
	end		
end

