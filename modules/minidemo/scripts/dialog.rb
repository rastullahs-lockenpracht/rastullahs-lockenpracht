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

