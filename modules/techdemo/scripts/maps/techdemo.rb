require 'globals.rb'
require 'player.rb'
require 'hero.rb'

# Physik aktivieren
$PM.setEnabled(true);

$CORE.log("init map 'minidemo'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "rl/dsa07")
$CORE.log("skybox set");

$CORE.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(0.8, -2.0, 3.0);
sunlight.getControlledObject().setCastShadows(false);
sunlight.getControlledObject().setDiffuseColour(0.8,0.8,0.7);
$CORE.log("Tageslicht erstellt.");

# DsaManager.getSingleton().loadDsaDataFile("kalchas.xml");

$CORE.log("Held erstellen");
$hero = Hero.new;
#hero = DsaManager.getSingleton().getPerson(10000);
#hero.setActor($AM.createMeshActor("Held","held.mesh", 2, -1.0));
$CORE.log("Held erstellt");
$CORE.log("Held in die Szene einfuegen.");
$hero.getActor().placeIntoScene(-300.0, 200.0, 400.0, 1.0, 0.0, 0.0, 0.0);
$CORE.log("Held eingefügt.");
PlayerSettings.preparePlayer($hero);
$CORE.log("Held vorbereitet.");
$UI.setActiveCharacter($hero);
$CORE.log("Held als aktiver Charakter gesetzt.");



$World.setFog( World::FOG_EXP, [0.8,0.8,1.0,0.5], 0.00003, 0.4, 1.0);

$CORE.log("map 'minidemo' initialisiert.");