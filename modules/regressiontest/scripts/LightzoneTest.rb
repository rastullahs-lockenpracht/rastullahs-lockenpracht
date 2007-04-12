require 'testcase.rb'

class LightzoneTest < TestCase
  def initialize(center)
    super(center)

    	# Define a base box to place everything else on.
      height = 0.1
      min_base = [-3, 0.05, -3]
      max_base = [3, height, 3]
      base = $AM.createBoxPrimitiveActor("LightzoneTestBase",
                                         min_base, max_base, "alpha_green")
      base.placeIntoScene(center);

      sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
      sunlight.getControlledObject().setDirection(0.7, -2.0, 0.8);
      sunlight.getControlledObject().setDiffuseColour(1, 1, 1);
      LightZoneManager.getSingleton().getDefaultLightZone().addLight(sunlight)

      interiorlight = $AM.createLightActor("interiorlight", LightObject::LT_POINT);
      interiorlight.getControlledObject().setDiffuseColour(1, 0, 0);
      interiorlight.placeIntoScene(center);
      interiorlight.setVisible(false)
      lzone = LightZoneManager.getSingleton().createLightZone(center, 3, RlScript::QUERYFLAG_PLAYER);
      lzone.addLight(interiorlight)
  end
end

