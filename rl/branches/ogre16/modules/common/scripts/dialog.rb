load "embed.rb"

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
