require 'globals.rb'
require 'player.rb'
require 'torch.rb'
require 'hero.rb'
require 'door.rb'
require 'switch.rb'

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
hero = Hero.new;
#hero = DsaManager.getSingleton().getPerson(10000);
#hero.setActor($AF.createMeshActor("Held","held.mesh", 2, -1.0));
$CORE.log("Held erstellt");
$CORE.log("Held in die Szene einfuegen.");
hero.getActor().placeIntoScene(0.0, 20.0, 40.0, 1.0, 0.0, 0.0, 0.0);
$CORE.log("Held eingefügt.");
PlayerSettings.preparePlayer(hero);
$CORE.log("Held vorbereitet.");
$UI.setActiveCharacter(hero);
$CORE.log("Held als aktiver Charakter gesetzt.");


$CORE.log("Türen einsetzen")
door1 = Door.new("Tuer_1", false, true);
door1.getActor().placeIntoScene(1168.0, -75.0, -110.0, 1.0, 0.0, 0.0, 0.0);
#, "Bone01");
door1.getActor().yaw(-90.0)

gtuer = Door.new("Tuer_2", false, false);
gtuer.getActor().placeIntoScene(1168.0, -75.0, 88.0, 1.0, 0.0, 0.0, 0.0)
#, "Bone01");
gtuer.getActor().yaw(-90.0)
$CORE.log("Türen fertig")


$CORE.log("Hebel reinsetzen")
hebel = Switch.new( "Hebel" );
hebel.getActor().placeIntoScene( 160.0, 24.0, 160.0, 1.0, 0.0, 0.0, 0.0 );
hebel.getActor().yaw(-90.0);
#p hebel
$CORE.log("Hebel fertig");


load "kreislauf.rb"

#Alles erzeugt, also Physik aktivieren
$PM.setEnabled(true);

$CORE.log("map 'minidemo' initialisiert.");