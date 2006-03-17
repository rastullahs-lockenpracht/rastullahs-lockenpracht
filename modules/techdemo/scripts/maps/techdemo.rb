load "embed.rb"

require 'player.rb'
require 'hero.rb'

# Physik aktivieren
$PM.setEnabled(true);

$SCRIPT.log("init map 'minidemo'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "rl/dsa07", 100.0)
$SCRIPT.log("skybox set");

$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(0.7, -2.0, 0.8);
sunlight.getControlledObject().setCastShadows(false);
sunlight.getControlledObject().setDiffuseColour(1,1,1);
# sunlight.getControlledObject().setDiffuseColour(0.8,0.8,0.7);
$SCRIPT.log("Tageslicht erstellt.");

# DsaManager.getSingleton().loadDsaDataFile("kalchas.xml");

$SCRIPT.log("Welten-Thema laden");
techdemoWelt = $AM.createSoundSampleActor( "techdemoWelt", "techdemo001.ogg" ); 
techdemoWelt.getControlledObject().set3d(false);
$SCRIPT.log(" Loopen");
techdemoWelt.getControlledObject().setLooping( true );
$SCRIPT.log(" Abspielen");
techdemoWelt.getControlledObject().play();
$SCRIPT.log("Welten-Thema fertig");

$SCRIPT.log("Held erstellen");
$hero = Hero.new;
$SCRIPT.log("Held erstellt");
$SCRIPT.log("Held in die Szene einfuegen.");
$hero.getActor().placeIntoScene([-69.220, 3.440, -4.000], [[0.0, 1.0, 0.0], 0.0]);
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
$bauer.getActor().placeIntoScene([29.9182, -0.6554, 9.590], [[0, 1, 0], 280])

load "techdemo_spinne.rb"
$waldspinne = Waldspinne.new()

load "techdemo_hoehle.rb"
$hebeltuer = Door.new("Eine Tür", "Eine Tür, allerdings ist keine Klinke vorhanden", false, false)
$hebeltuer.getActor().placeIntoScene([141.950, 7.200, 32.550], [[0, 1, 0], 30])
$hebel = Switch.new("HoehlenHebel")
$hebel.getActor().placeIntoScene([114.800, 10.700, 34.750], [[0, 1, 0], 280] )
$hebel.getActor().roll(-90.0);
$hebel.getActor().yaw(90.0);
$tueroeffner = CaveDoorOpener.new($hebeltuer, $hebel)

require "truhe.rb"
$truhe = Chest.new( "Truhe", false );
$truhe.getActor().placeIntoScene([123.530, 9.500, 47.250]);
$truhe.addItem(Torch.new("Fackel2"))

require "techdemo_hoehle_eingang.rb"
luftfels1 = Rock.new("nat_stein_gross_01.mesh", [105.510400390625, 26.0499194335938, 40.3499243164063], [[1.0, 0.0, 0.0], 0.0], false)
luftfels2 = Rock.new("nat_stein_gross_01.mesh", [99.82107421875, 21.3660180664063, 45.87017578125], [[1.0, 0.0, 0.0], 0.0], false)
luftfels3 = Rock.new("nat_stein_gross_01.mesh", [ 94.425087890625, 30.0978625488281, 44.10947265625], [[1.0, 0.0, 0.0], 0.0], false)

$SCRIPT.log("Steinschlag: SteinschlagzoneListener erstellen");
areaListener = SteinschlagzoneListener.new(luftfels1, luftfels2, luftfels3 );

$felshaufen = RockPile.new([0.0, 0.0, 0.0], [[1.0, 0.0, 0.0], 0.0], [ 94.425087890625, 10.0978625488281, 44.10947265625 ])
$felshaufen.getActor().getPhysicalThing().setContactListener( 
	RockpileContactListener.new($felshaufen, luftfels1, luftfels2, luftfels3) );


$SCRIPT.log("Steinschlag: Kugel-Zentrum Actor erstellen");
kugelDings = $AM.createEmptyActor( "Kugel-Zentrum" );
$SCRIPT.log("Steinschlag: Kugel-Zentrum Actor in die Szene einfügen");
kugelDings.placeIntoScene( [78.1769, 10.1317, 50.9391] );



$SCRIPT.log("Steinschlag: SteinschlagzoneListener hinzufügen");
$GameEveMgr.addSphereAreaListener( kugelDings, 4.000, areaListener, Actor::QGF_PLAYER );

$SCRIPT.log("map 'techdemo' initialisiert.");
$UI.toggleCharacterStateWindow()
