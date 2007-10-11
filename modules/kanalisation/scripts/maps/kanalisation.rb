load "embed.rb"
$World = $CORE.getWorld()
require 'player.rb'
require 'hero.rb'
require 'mckhero.rb'
require 'door.rb'

# Physik aktivieren
$PM.setEnabled(true);

$CORE.getWorld().setSkyBox(true, "rl/dsa07", 100.0)

#$SCRIPT.log("Tageslicht erstellen..");
#sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
#sunlight.getControlledObject().setDirection(-0.17101, -0.873647, 0.969846);
#sunlight.getControlledObject().setCastShadows(true);

#$hero.getActor().placeIntoScene([0.0, 0.0, 0.0]);
#sunlight.getControlledObject().setSpecularColour(1.0,1.0,1.0);

#sunlight.getControlledObject().setDiffuseColour(0.1,0.1,0.2);
$SCRIPT.log("Tageslicht erstellt.");


#backlight = $AM.createLightActor("backlight", LightObject::LT_DIRECTIONAL);
#backlight.getControlledObject().setDirection(0.17101, 0.873647, -0.969846);
#backlight.getControlledObject().setCastShadows(false);
#backlight.getControlledObject().setDiffuseColour(-0.3,-0.3,-0.3);



$World.setFog( World::FOG_EXP, [0.658,0.7568,0.9137,1.0], 0.00049, 0.8, 4.0);
$World.setAmbientLight( 0.08, 0.06, 0.05  );


$SCRIPT.log("Create hero")
#$hero = $GOM.createGameObject("hero", 1);
$hero = $GOM.createGameObject("mckhero", 1);

$SCRIPT.log("Place hero into scene")
$hero.placeIntoScene();
$hero.setPosition([-3.86,3.80,-1.60]);

$SCRIPT.log("Prepare hero for being the active character")
PlayerSettings.preparePlayer($hero)

$SCRIPT.log("Set hero as active character")
$UI.setActiveCharacter($hero)

$SCRIPT.log("Hero ready")


#$SCRIPT.log("Held in die Szene einfuegen.");#-22.0130577087402, -6.70670890808105, 25.1438484191895
#$hero.getActor().placeIntoScene([0.0, 20.0, 0.0]);


$SCRIPT.log("Türen einsetzen");
#door1 = Door.new("Haustuer", "Eine Holztür", [-12.9124, -6.86492, 13.7133], [0, -120, 0], "arc_door_02.mesh");
#door1.addActions(false, true)

#door2 = Door.new("Tavernentür", "Die Eingangstür zur Taverne", [52.461, 1.28902, -26.323], [-0.462671,0.0, 0.88653, 0.0], "arc_door_tavern.mesh");
#door2.addActions(false, true)

$SCRIPT.log("Türen gesetzt");

#####################

#BoxActor = $AM.createMeshActor("Box", "facial.mesh" );
#$SCRIPT.log("actor erstellt");
 
#BoxActor.placeIntoScene(0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0);
#$SCRIPT.log("actor in scene gesetzt");
#BoxAnim = BoxActor.getControlledObject().startAnimation("Speak");
#$SCRIPT.log("animation abgespielt.");

##########################

load "particle.rb"
load "equip.rb"