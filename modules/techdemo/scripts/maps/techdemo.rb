require 'globals.rb'
require 'player.rb'
require 'hero.rb'

# Physik aktivieren
$PM.setEnabled(true);

$SCRIPT.log("init map 'minidemo'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "rl/dsa07")
$SCRIPT.log("skybox set");

$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(0.7, -2.0, 0.8);
sunlight.getControlledObject().setCastShadows(false);
sunlight.getControlledObject().setDiffuseColour(0.8,0.8,0.7);
$SCRIPT.log("Tageslicht erstellt.");

# DsaManager.getSingleton().loadDsaDataFile("kalchas.xml");

$SCRIPT.log("Held erstellen");
$hero = Hero.new;
#hero = DsaManager.getSingleton().getPerson(10000);
#hero.setActor($AM.createMeshActor("Held","held.mesh", 2, -1.0));
$SCRIPT.log("Held erstellt");
$SCRIPT.log("Held in die Szene einfuegen.");
$hero.getActor().placeIntoScene(-300.0, 200.0, 400.0, 1.0, 0.0, 0.0, 0.0);
$SCRIPT.log("Held eingefügt.");
PlayerSettings.preparePlayer($hero);
$SCRIPT.log("Held vorbereitet.");
$UI.setActiveCharacter($hero);
$SCRIPT.log("Held als aktiver Charakter gesetzt.");



$World.setFog( World::FOG_EXP, [0.8,0.8,1.0,0.5], 0.00003, 0.4, 1.0);

load "techdemo_umgebung.rb"
load "techdemo_quests.rb"


load "techdemo_bauer.rb"
$bauer = BauerArnolfGrossschaedel.new()
$bauer.getActor().placeIntoScene(3161.82, -65.54, 982.0, -0.767904, 0.0, 0.640565, 0.0)

load "techdemo_spinne.rb"
$waldspinne = Waldspinne.new()
$waldspinne.getActor().placeIntoScene(-2040.0, 343.0, -8200.0, 1.0, 0.0, 0.0, 0.0)	
$waldspinne.getActor().setScale(5.0, 5.0, 5.0)

load "techdemo_hoehle.rb"
$hebeltuer = Door.new("Eine Tür", "Eine Tür, allerdings ist keine Klinke vorhanden", false, false)
$hebeltuer.getActor().placeIntoScene(13270.0, 906.0, 3746.0, 0.965926, 0.0, 0.258819, 0.0)
$hebel = Switch.new("HoehlenHebel")
$hebel.getActor().placeIntoScene(12153.0, 1050.0, 4725.0, 0.707107, 0.707107, 0.0, 0.0)
$tueroeffner = CaveDoorOpener.new($hebeltuer, $hebel)

require "truhe.rb"
$truhe = Chest.new( "Truhe", false );
$truhe.getActor().placeIntoScene(12353.0, 950.0, 4725.0, 1.0, 0.0, 0.0, 0.0 );
$truhe.getActor().yaw(-90.0);
$truhe.addItem(Torch.new("Fackel2"))

require "techdemo_hoehle_eingang.rb"
#$kleinefelsen = RockManager.new()
#$grossefelsen = RockManager.new()
#$grossefelsen.add(Rock.new("nat_stein_gross_01.mesh", [9300.0, 1088.0, 4113.0], [1.0, 0.0, 0.0, 0.0], true))
#$grossefelsen.add(Rock.new("nat_stein_gross_01.mesh", [9450.0, 1020.0, 4900.0], [1.0, 0.0, 0.0, 0.0], true))
#$luftfels = Rock.new("nat_stein_gross_01.mesh", [9300.0, 2088.0, 4113.0], [1.0, 0.0, 0.0, 0.0], true)
#$grossefelsen.add($luftfels)

$felshaufen = RockPile.new([0.0, 0.0, 0.0], [1.0, 0.0, 0.0, 0.0], [9300.0, 1088.0, 4113.0])

$SCRIPT.log("Steinschlag: Kugel-Zentrum Actor erstellen");
kugelDings = $AM.createEmptyActor( "Kugel-Zentrum" );
$SCRIPT.log("Steinschlag: Kugel-Zentrum Actor in die Szene einfügen");
kugelDings.placeIntoScene( 7817.69, 1013.17, 5093.91, 1.0, 0.0, 0.0, 0.0);

$SCRIPT.log("Steinschlag: SteinschlagzoneListener erstellen");
areaListener = SteinschlagzoneListener.new($felshaufen);

$SCRIPT.log("Steinschlag: SteinschlagzoneListener hinzufügen");
$GameEveMgr.addSphereAreaListener( kugelDings, 400.0, areaListener, Actor::QGF_PLAYER );

$SCRIPT.log("map 'techdemo' initialisiert.");
