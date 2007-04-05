load "embed.rb"
$World = $CORE.getWorld()
require 'player.rb'
require 'hero.rb'
require 'door.rb'

# Physik aktivieren
$PM.setEnabled(true);

$CORE.getWorld().setSkyBox(true, "rl/dsa07", 100.0)

$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(-0.17101, -0.873647, 0.969846);
sunlight.getControlledObject().setCastShadows(false);

sunlight.getControlledObject().setDiffuseColour(1.0,1.0,1.0);
sunlight.getControlledObject().setSpecularColour(1.0,1.0,1.0);

#sunlight.getControlledObject().setDiffuseColour(0.1,0.1,0.2);
$SCRIPT.log("Tageslicht erstellt.");


#backlight = $AM.createLightActor("backlight", LightObject::LT_DIRECTIONAL);
#backlight.getControlledObject().setDirection(0.17101, 0.873647, -0.969846);
#backlight.getControlledObject().setCastShadows(false);
#backlight.getControlledObject().setDiffuseColour(-0.3,-0.3,-0.3);





$World.setFog( World::FOG_EXP, [0.4,0.5,0.6,0.7], 0.00219, 0.5, 1.0);
#$World.setAmbientLight( 0.3, 0.3, 0.3  );


$SCRIPT.log("Create hero")
$hero = $GOM.createGameObjectProxy("hero", 1).getGameObject();

$SCRIPT.log("Place hero into scene")
$hero.placeIntoScene();
$hero.setPosition([-16.18,9.17,29.55]);

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

$SCRIPT.log("Dorf-Thema laden");
testmaptrack = $AM.createSoundSampleActor( "testmaptrack", "testmaptrack.ogg" ); 
testmaptrack.getControlledObject().set3d(false);
$SCRIPT.log(" Loopen");
testmaptrack.getControlledObject().setLooping( true );
$SCRIPT.log(" Laden");
testmaptrack.getControlledObject().load();
$SCRIPT.log("Dorf-Thema fertig");

testmaptrack.getControlledObject().play();


load "particle.rb"
