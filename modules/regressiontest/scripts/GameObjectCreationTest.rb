require 'testcase.rb'

class GameObjectCreationTest < TestCase
  def execute()
	$SCRIPT.log("GameObjectCreationTest start");
    
	appl = $GOM.createGameObject("apple");
    appl.setPosition(rel_pos([0,0,0]));
	appl.placeIntoScene()

    appl2 = $GOM.createGameObject("apple", 15);
    appl2.setPosition(rel_pos([0,1,0]));
	appl2.placeIntoScene()
	
	appl3 = $GOM.createGameObject("apple2");
    appl3.setPosition(rel_pos([0,0,1]));
	appl3.placeIntoScene()
	assertEquals(appl3.getProperty("meshfile"), "sphere.1m.mesh", "mesh file")
	assertEquals(appl3.getProperty("description"), "Ein Apfel. Definitiv.", "description")
	assertEquals(appl3.getProperty("mass"), 75, "property mass")
	
	$SCRIPT.log("GameObjectCreationTest finished");
  end
end