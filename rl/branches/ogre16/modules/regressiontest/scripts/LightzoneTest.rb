require 'testcase.rb'
require 'areahelper.rb'

class LightzoneTest < TestCase
  def execute()
    	# Define a base box to place everything else on.
      height = 0.1
      min_base = [-3, 0.05, -3]
      max_base = [3, height, 3]
      base = $AM.createBoxPrimitiveActor("LightzoneTestBase",
                                         min_base, max_base, "alpha_gray")
      base.placeIntoScene(getCenter());
	  
	  MapLoader.new("regressiontest").loadMap("regressiontest_lights.rlmap.xml")

	  # Test if an actor is in the zone
	  ball = $GOM.createGameObject("apple");
	  # should be inside zone
	  ball.setPosition([-9, 0.5, -5]);
	  ball.placeIntoScene();
	  zm = ZoneManager.getSingleton();
	  if( not zm.isInZone(zm.getZone("Testzone"), ball.getActor()) )
		  p "apple not found in Testzone!"
	  end

	  # test scripted zones
	  _CreateOneTimeTrigger("TestTrigger", PhysicsManager::GT_BOX, [4,1,5], [0,0,0,1], [3,3,2], %q{print "OneTimeTestTrigger"})


	  $SCRIPT.log("LightzoneTest initialisiert")
  end
end

