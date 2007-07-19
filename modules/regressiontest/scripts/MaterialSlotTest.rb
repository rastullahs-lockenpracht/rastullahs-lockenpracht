require 'globals.rb'
require 'gameobjectprops.rb'

class Clothing < Item
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

class MaterialSlotTest < TestCase
    def execute()
        test_obj1 = $GOM.createGameObject("mckhero")
        test_obj1.setPosition(rel_pos([0, 0, 0]))
        test_obj1.placeIntoScene();
        test_item1 = $GOM.createGameObject("Clothes #1")
        test_obj1.getInventory().hold(test_item1, "Torso")

        test_obj2 = $GOM.createGameObject("mckhero")
        test_obj2.setPosition(rel_pos([2, 0, 0]))
        test_obj2.placeIntoScene();
        test_item2 = $GOM.createGameObject("Clothes #2")
        test_obj2.getInventory().hold(test_item2, "Torso")
    end
end
