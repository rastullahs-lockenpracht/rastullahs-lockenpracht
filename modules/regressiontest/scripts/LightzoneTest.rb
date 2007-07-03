require 'testcase.rb'

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

	  $SCRIPT.log("LightzoneTest initialisiert")
  end
end

