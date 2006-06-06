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



class DialogCharTest < DialogCharacter
	def initialize()
		super()
		print("DialogCharacter nach neuem DialogSystem erzeugt")
	end

	def calcOptionValue( optioName )
		print("Calc Option Value")
		return 4
	end

	def calcResponseValue( responseName )
		print("Calc Response Value")
		return 4
	end

	def calcSelectionValue( selectionName )
		print("Calc Selection Value")
		return 4
	end
end
