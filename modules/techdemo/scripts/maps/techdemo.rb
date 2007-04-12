load "embed.rb"

require 'player.rb'
require 'hero.rb'
require 'npc.rb'

# Physik aktivieren
$PM.setEnabled(true);

$SCRIPT.log("init map 'techdemo'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "rl/dsa07", 100.0)
$SCRIPT.log("skybox set");

$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(0.7, -2.0, 0.8);
#sunlight.getControlledObject().setCastShadows(false);
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

$SCRIPT.log("Create hero")
$hero = $GOM.createGameObjectProxy("hero", 1).getGameObject();

$SCRIPT.log("Place hero into scene")
$hero.setPosition([0, 0.7, 0])
$hero.placeIntoScene()

$SCRIPT.log("Prepare hero for being the active character")
PlayerSettings.preparePlayer($hero)

$SCRIPT.log("Set hero as active character")
$UI.setActiveCharacter($hero)

$World.setFog( World::FOG_EXP, [0.8,0.8,1.0,0.5], 0.00003, 0.4, 1.0);

load "techdemo_umgebung.rb"
load "techdemo_quests.rb"
load "techdemo_spinne.rb"
load "techdemo_hoehle.rb"

$bauer = $GOM.createGameObjectProxy("ArnolfGrossschaedel").getGameObject()
$bauer.setPosition([29.9182, -0.6554, 9.590])
$bauer.setOrientation([[0, 1, 0], 280])
$bauer.placeIntoScene();

$waldspinne = $GOM.createGameObjectProxy("Waldspinne").getGameObject()
$waldspinne.setPosition([-20.400, 3.430, -82.000])
$waldspinne.placeIntoScene();
$waldspinne.getActor().setScale(5.0, 5.0, 5.0)


$hebeltuer = $GOM.createGameObjectProxy("door").getGameObject();
$hebeltuer.setPosition([141.95, 7.20, 32.55]);
$hebeltuer.setOrientation([[0, 1, 0], 30]);
$hebeltuer.setDescription("Eine Tür, allerdings ist keine Klinke vorhanden")
$hebeltuer.placeIntoScene()
# $hebeltuer.addActions(false, false)

$hebel = $GOM.createGameObjectProxy("switch3way").getGameObject();
$hebel.setPosition([114.800, 10.700, 34.750])
$hebel.setOrientation([[0, 1, 0], 280])
$hebel.placeIntoScene()
$hebel.getActor().roll(-90.0);
$hebel.getActor().yaw(90.0);
$tueroeffner = CaveDoorOpener.new($hebeltuer, $hebel)

require "truhe.rb"
$truhe = $GOM.createGameObjectProxy("chest").getGameObject();
$truhe.setPosition([123.530, 9.500, 47.250])
$truhe.placeIntoScene();
$truhe.addItem($GOM.createGameObjectProxy("torch").getGameObject())

require "techdemo_hoehle_eingang.rb"

trank = $GOM.createGameObjectProxy("Heiltrank").getGameObject()
trank.setPosition([155.59, 6.75, 20.96]);
trank.placeIntoScene();

luftfels1 = $GOM.createGameObjectProxy("Fels").getGameObject()
luftfels1.setPosition([105.51, 26.05, 40.35])
luftfels2 = $GOM.createGameObjectProxy("Fels").getGameObject()
luftfels2.setPosition([99.82, 21.37, 45.87])
luftfels3 = $GOM.createGameObjectProxy("Fels").getGameObject()
luftfels3.setPosition([94.43, 30.1, 44.11])

$SCRIPT.log("Steinschlag: SteinschlagzoneListener erstellen");
areaListener = SteinschlagzoneListener.new(luftfels1, luftfels2, luftfels3 );

$felshaufen = $GOM.createGameObjectProxy("rockpile").getGameObject()
$felshaufen.setPosition([94.43, 10.1, 44.11]);
$felshaufen.placeIntoScene()
$felshaufen.getActor().getPhysicalThing().setContactListener( 
	RockpileContactListener.new($felshaufen, luftfels1, luftfels2, luftfels3) );


$SCRIPT.log("Steinschlag: Kugel-Zentrum Actor erstellen");
kugelDings = $AM.createEmptyActor( "Kugel-Zentrum" );
$SCRIPT.log("Steinschlag: Kugel-Zentrum Actor in die Szene einfügen");
kugelDings.placeIntoScene( [78.1769, 10.1317, 50.9391] );



$SCRIPT.log("Steinschlag: SteinschlagzoneListener hinzufügen");
$GameEveMgr.addSphereAreaListener( kugelDings, 4.000, areaListener, RlScript::QUERYFLAG_PLAYER );

$SCRIPT.log("map 'techdemo' initialisiert.");
$WF.toggleCharacterStateWindow()
