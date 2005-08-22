require 'globals.rb'
require 'player.rb'
require 'torch.rb'
require 'hero.rb'
require 'door.rb'
require 'switch.rb'
require 'truhe.rb'
require 'door_opener.rb'
require 'heiltrank.rb'

# Der GC reisst oft die Objekte ab
GC.disable

# Physik aktivieren
$PM.setEnabled(true);

$CORE.log("init map 'minidemo'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "rl/dsa07")
$CORE.log("skybox set");

$CORE.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(-0.8, -2.0, 3.0);
sunlight.getControlledObject().setCastShadows(false);
sunlight.getControlledObject().setDiffuseColour(0.8,0.8,0.7);
$CORE.log("Tageslicht erstellt.");

DsaManager.getSingleton().loadDsaDataFile("kalchas.xml");

$CORE.log("Held erstellen");
$hero = Hero.new;
#hero = DsaManager.getSingleton().getPerson(10000);
#hero.setActor($AM.createMeshActor("Held","held.mesh", 2, -1.0));
$CORE.log("Held erstellt");
$CORE.log("Held in die Szene einfuegen.");
$hero.getActor().placeIntoScene(0.0, 20.0, 40.0, 1.0, 0.0, 0.0, 0.0);
$CORE.log("Held eingefügt.");
PlayerSettings.preparePlayer($hero);
$CORE.log("Held vorbereitet.");
$UI.setActiveCharacter($hero);
$CORE.log("Held als aktiver Charakter gesetzt.");


$CORE.log("Türen einsetzen")
door1 = Door.new("Tuer_1", false, true);
door1.getActor().placeIntoScene(1368.0, -75.0, -170.0, 1.0, 0.0, 0.0, 0.0);
#, "Bone01");
door1.getActor().yaw(-90.0)

gtuer = Door.new("Tuer_2", false, false);
gtuer.getActor().placeIntoScene(1368.0, -75.0, 60.0, 1.0, 0.0, 0.0, 0.0)
#, "Bone01");
gtuer.getActor().yaw(-90.0)
$CORE.log("Türen fertig")


$CORE.log("Hebel reinsetzen")
hebel = Switch.new( "Hebel" );
hebel.getActor().placeIntoScene( 1330.0, 25.0, 180.0, 1.0, 0.0, 0.0, 0.0 );
hebel.getActor().pitch(90.0)
hebel.getActor().roll(90.0)
$CORE.log("Hebel fertig");

doorprocessor = SecretDoorOpener.new(gtuer, hebel)

$CORE.log("Truhe reinsetzen")
chest = Chest.new( "Truhe", false, true );
chest.getActor().placeIntoScene( 1750.0, -350.0, -105.0, 1.0, 0.0, 0.0, 0.0 );
chest.getActor().yaw(-90.0);
$CORE.log("Truhe fertig");

$CORE.log("Trank reinsetzen")
trank = Heiltrank.new( 28, "Trank", "Eine Flasche mit einer geheimnisvollen Fluessigkeit", "obj_heiltrank01.mesh", "A" );
chest.addItem(trank);
$CORE.log("Trank fertig");


load "kreislauf.rb"

load "area.rb"

$CORE.log("map 'minidemo' initialisiert.");
