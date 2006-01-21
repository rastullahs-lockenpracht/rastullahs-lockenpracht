load "embed.rb"

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
#hero = DsaManager.getSingleton().getPerson(10000);
#hero.setActor($AM.createMeshActor("Held","held.mesh", 2, -1.0));
$SCRIPT.log("Held erstellt");
$SCRIPT.log("Held in die Szene einfuegen.");
$hero.getActor().placeIntoScene(-6922.0, 344.0, -400.0, 1.0, 0.0, 0.0, 0.0);
#$hero.getActor().placeIntoScene( 12353.0, 950.0, 4725.0, 1.0, 0.0, 0.0, 0.0);
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
$bauer.getActor().placeIntoScene(2991.82, -65.54, 959.0, -0.767904, 0.0, 0.640565, 0.0)

load "techdemo_spinne.rb"
$waldspinne = Waldspinne.new()

load "techdemo_hoehle.rb"
$hebeltuer = Door.new("Eine Tür", "Eine Tür, allerdings ist keine Klinke vorhanden", false, false)
$hebeltuer.getActor().placeIntoScene(14195.0, 720.0, 3255.0, 0.965926, 0.0, 0.8, 0.0)
$hebel = Switch.new("HoehlenHebel")
$hebel.getActor().placeIntoScene(11480.0, 1070.0, 3475.0, -0.767904, 0.0, 0.5, 0.0 )
$hebel.getActor().roll(-90.0);$hebel.getActor().yaw(90.0);
$tueroeffner = CaveDoorOpener.new($hebeltuer, $hebel)

require "truhe.rb"
$truhe = Chest.new( "Truhe", false );
$truhe.getActor().placeIntoScene(12353.0, 950.0, 4725.0, 1.0, 0.0, 0.0, 0.0 );
$truhe.addItem(Torch.new("Fackel2"))

require "techdemo_hoehle_eingang.rb"
luftfels1 = Rock.new("nat_stein_gross_01.mesh", [10551.0400390625, 2604.99194335938, 4034.99243164063], [1.0, 0.0, 0.0, 0.0], false)
luftfels2 = Rock.new("nat_stein_gross_01.mesh", [9982.107421875, 2136.60180664063, 4587.017578125], [1.0, 0.0, 0.0, 0.0], false)
luftfels3 = Rock.new("nat_stein_gross_01.mesh", [ 9442.5087890625, 3009.78625488281, 4410.947265625], [1.0, 0.0, 0.0, 0.0], false)

$SCRIPT.log("Steinschlag: SteinschlagzoneListener erstellen");
areaListener = SteinschlagzoneListener.new(luftfels1, luftfels2, luftfels3 );

$felshaufen = RockPile.new([0.0, 0.0, 0.0], [1.0, 0.0, 0.0, 0.0], [ 9442.5087890625, 1009.78625488281, 4410.947265625 ])
$felshaufen.getActor().getPhysicalThing().setContactListener( 
	RockpileContactListener.new($felshaufen, luftfels1, luftfels2, luftfels3) );


$SCRIPT.log("Steinschlag: Kugel-Zentrum Actor erstellen");
kugelDings = $AM.createEmptyActor( "Kugel-Zentrum" );
$SCRIPT.log("Steinschlag: Kugel-Zentrum Actor in die Szene einfügen");
kugelDings.placeIntoScene( 7817.69, 1013.17, 5093.91, 1.0, 0.0, 0.0, 0.0);



$SCRIPT.log("Steinschlag: SteinschlagzoneListener hinzufügen");
$GameEveMgr.addSphereAreaListener( kugelDings, 400.0, areaListener, Actor::QGF_PLAYER );

$SCRIPT.log("map 'techdemo' initialisiert.");
$UI.toggleCharacterStateWindow()
