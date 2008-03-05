require 'globals.rb'
require 'gameobjectprops.rb'

class Clothing < Armor
	include GameObjectProperties

	def setProperty(key, value)
		if (key == "material")
			@_prop_material = value
		else
		   super(key, value)
		end
		
	end

	def getProperty(key)
		if (key == "material")
			return @_prop_material
		else
		    return super(key)
		end
		
	end
end