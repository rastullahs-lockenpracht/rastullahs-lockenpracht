load "embed.rb"
$World = $CORE.getWorld()
require 'player.rb'
require 'hero.rb'
require 'mckhero.rb'
require 'door.rb'
require 'torch.rb'


loader = MapLoader.new("kanalisation");
loader.loadMap("kana_lights.rlmap.xml");

# Physik aktivieren
$PM.setEnabled(true);

$SCRIPT.log("rauschendesWasser geladen");

#$hero.getActor().placeIntoScene([0.0, 0.0, 0.0]);
#$SCRIPT.log("Tageslicht erstellen..");
#sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
#$hero.getActor().placeIntoScene([0.0, 0.0, 0.0]);
#sunlight.getControlledObject().setCastShadows(true);

#$hero.getActor().placeIntoScene([0.0, 0.0, 0.0]);
#sunlight.getControlledObject().setSpecularColour(1.0,1.0,1.0);

#sunlight.getControlledObject().setDiffuseColour(0.1,0.1,0.2);
$SCRIPT.log("Tageslicht erstellt.");


backlight = $AM.createLightActor("backlight", LightObject::LT_DIRECTIONAL);
backlight.getControlledObject().setDirection(0.17101, 0.873647, -0.969846);
backlight.getControlledObject().setCastShadows(false);
backlight.getControlledObject().setDiffuseColour(-0.3,-0.3,-0.3);



$World.setFog( World::FOG_EXP, [0.658,0.7568,0.9137,1.0], 0.00049, 0.8, 1.0);
$World.setAmbientLight( 0.08, 0.06, 0.05  );

$SCRIPT.log("Sound laden");
kanalLied = $AM.createSoundStreamActor( "KanalLied", "kanal.ogg" ); 
kanalLied.getControlledObject().set3d(false);
$SCRIPT.log(" Loopen");
kanalLied.getControlledObject().setLooping( true );
$SCRIPT.log(" Abspielen");
kanalLied.getControlledObject().play();
$SCRIPT.log("Sound fertig");

$SCRIPT.log("Create hero")
#$hero = $GOM.createGameObject("hero", 1);
$hero = $GOM.createGameObject("mckhero", 1);

$SCRIPT.log("Place hero into scene")
$hero.placeIntoScene();
$hero.setPosition([9.45,4.14,9.57]);

$SCRIPT.log("Prepare hero for being the active character")
PlayerSettings.preparePlayer($hero)

$SCRIPT.log("Set hero as active character")
$UI.setActiveCharacter($hero)

$SCRIPT.log("Hero ready")

#$SCRIPT.log("Held in die Szene einfuegen.");#-22.0130577087402, -6.70670890808105, 25.1438484191895
#$hero.getActor().placeIntoScene([0.0, 20.0, 0.0]);

#$SCRIPT.log("Fackellicht erstellen..");
#fackellicht = $AM.createLightActor("Das Licht der Fackel", LightObject::LT_POINT );
#fackellicht.getControlledObject().setCastShadows(false);
#fackellicht.getControlledObject().setDiffuseColour(0.4,0.3,0.2);
#fackellicht.getControlledObject().setAttenuation(2.0, 0.79,  -0.21, 0.268 );
#$hero.getActor().attachToSlot( fackellicht, "Bip01 R SlotHand" );
#$SCRIPT.log("Fackellicht erstellt.");


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