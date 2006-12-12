load "embed.rb"
$World = $CORE.getWorld()
require 'player.rb'
require 'hero.rb'

# Physik aktivieren
$PM.setEnabled(true);

$CORE.getWorld().setSkyBox(true, "rl/dsa07", 100.0)

$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(-0.17101, -0.873647, 0.969846);
sunlight.getControlledObject().setCastShadows(true);

sunlight.getControlledObject().setDiffuseColour(1.0,1.0,1.0);
#sunlight.getControlledObject().setDiffuseColour(0.1,0.1,0.2);
$SCRIPT.log("Tageslicht erstellt.");


backlight = $AM.createLightActor("backlight", LightObject::LT_DIRECTIONAL);
backlight.getControlledObject().setDirection(0.17101, 0.873647, -0.969846);
backlight.getControlledObject().setCastShadows(true);
backlight.getControlledObject().setDiffuseColour(-0.3,-0.3,-0.3);


tavernfire1 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire1.placeIntoScene( 94.28, 5.65, -30.88, 1.0, 0.0, 0.0, 0.0 );


$World.setFog( World::FOG_EXP, [0.4,0.5,0.6,0.7], 0.00219, 0.5, 1.0);
$World.setAmbientLight( 0.2, 0.2, 0.2  );

$SCRIPT.log("Held erstellen");
$hero = Hero.new;
$SCRIPT.log("Held erstellt");
$SCRIPT.log("Held in die Szene einfuegen.");#-22.0130577087402, -6.70670890808105, 25.1438484191895
$hero.getActor().placeIntoScene([0.0, 20.0, 0.0]);
$SCRIPT.log("Held eingefügt.");
PlayerSettings.preparePlayer($hero);
$SCRIPT.log("Held vorbereitet.");
$UI.setActiveCharacter($hero);
$SCRIPT.log("Held als aktiver Charakter gesetzt.");
$heroVehicle = $AI.createAgent(AgentManager::AGENT_PLAYER, $hero);	
$SCRIPT.log("SteeringVehicle für Held erstellen...");


$SCRIPT.log("Dorf-Thema laden");
testmaptrack = $AM.createSoundSampleActor( "testmaptrack", "testmaptrack.ogg" ); 
testmaptrack.getControlledObject().set3d(false);
$SCRIPT.log(" Loopen");
testmaptrack.getControlledObject().setLooping( true );
$SCRIPT.log(" Laden");
testmaptrack.getControlledObject().load();
$SCRIPT.log("Dorf-Thema fertig");

testmaptrack.getControlledObject().play();
