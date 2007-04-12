require 'testcase.rb'

class Lightzone < GameAreaListener
  def initialize()
    super()
  end

  def areaLeft(anEvent)
   p "left"
  end

  def areaEntered(anEvent)
   p "entered"
  end
end

class LightzoneTest < TestCase
  def initialize(center)
    super(center)

    	# Define a base box to place everything else on.
      height = 0.1
      min_base = [-10, 0.05, -10]
      max_base = [10, height, 10]
      base = $AM.createBoxPrimitiveActor("LightzoneTestBase",
                                         min_base, max_base, "alpha_green")
      base.placeIntoScene(center);
      
      
      # Add a zone trigger mesh
      lzone = Lightzone.new();
      kugelDings = $AM.createEmptyActor( "Light zone center" );
	    kugelDings.placeIntoScene( center );
	    $GameEveMgr.addSphereAreaListener( kugelDings, 10.000, lzone, Actor::QGF_PLAYER );
  end
end

