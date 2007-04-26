require 'testcase.rb'

class LightzoneTest < TestCase
  def initialize(center)
    super(center)

    	# Define a base box to place everything else on.
      height = 0.1
      min_base = [-3, 0.05, -3]
      max_base = [3, height, 3]
      base = $AM.createBoxPrimitiveActor("LightzoneTestBase",
                                         min_base, max_base, "alpha_gray")
      base.placeIntoScene(center);
	  
	  MapLoader.new("regressiontest").loadMap("regressiontest_lights.rlmap.xml")

  end
end

