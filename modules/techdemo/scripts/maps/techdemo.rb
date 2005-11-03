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
sunlight.getControlledObject().setDirection(0.7, -2.0, 0.8);
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

load "techdemo_umgebung.rb"


load "techdemo_bauer.rb"
$bauer = BauerArnolfGrossschaedel.new()
$bauer.getActor().placeIntoScene(3161.82, -65.54, 982.0, 0.767904, 0.0, 0.640565, 0.0)

load "techdemo_spinne.rb"
$waldspinne = Waldspinne.new()
$waldspinne.getActor().placeIntoScene(-2040.0, 343.0, -8200.0, 1.0, 0.0, 0.0, 0.0)	
$waldspinne.getActor().setScale(5.0, 5.0, 5.0)

load "techdemo_hoehle.rb"
$hebeltuer = Door.new("Eine Tür", "Eine Tür, allerdings ist keine Klinke vorhanden", false, false)
$hebeltuer.getActor().placeIntoScene(13270.0, 906.0, 3746.0, 0.965926, 0.0, 0.258819, 0.0)
$hebel = Switch.new("HoehlenHebel")
$hebel.getActor().placeIntoScene(12153.0, 1050.0, 4725.0, 0.707107, 0.707107, 0.0, 0.0)
$hebel.

$tueroeffner = CaveDoorOpener.new($hebeltuer, $hebel)


$CORE.log("map 'techdemo' initialisiert.");
