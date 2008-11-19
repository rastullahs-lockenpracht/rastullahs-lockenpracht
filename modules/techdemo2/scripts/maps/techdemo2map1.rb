load "embed.rb"

require 'player.rb'
require 'hero.rb'
require 'door.rb'

# Physik aktivieren
$PM.setEnabled(true);

$SCRIPT.log("init map 'Techdemo2'...");

$SCRIPT.log("Held erstellen");
$hero = $GOM.createGameObject("hero", 1);
$hero.placeIntoScene()
PlayerSettings.preparePlayer($hero)
PartyManager.getSingleton().setActiveCharacter($hero)
$SCRIPT.log("Held erstellt");

#$heroVehicle = $AGM.createAgent(AgentManager::AGENT_PLAYER, $hero);	
#$SCRIPT.log("SteeringVehicle für Held erstellen...");


$World = $CORE.getWorld();
$World.setSkyBox(true, "rl/desert07", 100.0);
$SCRIPT.log("skybox set");


$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(-0.17101, -0.873647, 0.969846);
sunlight.getControlledObject().setCastShadows(true);
sunlight.getControlledObject().setDiffuseColour(0.8,0.8,0.8);
$SCRIPT.log("Tageslicht erstellt.");


$SCRIPT.log("Türen einsetzen");
door1 = $GOM.createGameObject("door")
door1.setMeshfile("arc_door_02.mesh");
door1.setPosition([-12.9124, -6.86492, 13.7133]);
door1.setOrientation([0, -120, 0]);
door1.setDescription("Eine Holztür");

door2 = $GOM.createGameObject("door")
door2.setMeshfile("arc_door_02.mesh");
door2.setPosition([-34.6458, -6.2058, 14.9355]);
door2.setOrientation([0, -212.5, 0]);
door2.setDescription("Eine Holztür");

door1.placeIntoScene();
door2.placeIntoScene();
$SCRIPT.log("Türen gesetzt");


$SCRIPT.log("Dorf-Thema laden");
techdemoDorf = $AM.createSoundSampleActor( "techdemoDorf", "techdemo001.ogg" ); 
techdemoDorf.getControlledObject().set3d(false);
$SCRIPT.log(" Loopen");
techdemoDorf.getControlledObject().setLooping( true );
$SCRIPT.log(" Laden");
#techdemoDorf.getControlledObject().load();
$SCRIPT.log("Dorf-Thema fertig");

$World.setFog( World::FOG_EXP, [0.1,0.08,0.01,0.1], 0.00503, 10.0, 100.0);

$SCRIPT.log("Quests laden");
load( "techdemo2_quests.rb" );
load( "techdemo2_buch.rb" );
$SCRIPT.log("Quests geladen");

$SCRIPT.log("SteeringBehaviours laden");
load("steering.rb");

$SCRIPT.log("NSCs laden");
load( "sonja.rb" );
load( "jarn.rb" );
load( "tsafried.rb" );
load( "bauer.rb" );
load( "wirt.rb" );
load( "forrest.rb");
load( "wolframstuer.rb" );
$SCRIPT.log("NSCs geladen");

$SCRIPT.log("Weiteres laden");
load( "viecher.rb" );
load( "buchderkultur.rb" );
$SCRIPT.log("Weiteres geladen");

#Musik als letztes Starten
#techdemoDorf.getControlledObject().play();

# Tagebucheintrag
entry = $tagebuchtexte['dorf1']
RulesSubsystem.getSingleton().getQuestBook().addJournalEntry( entry[0], entry[1] );


#******** mapchange - abhängig vom Questzustand **********
$SCRIPT.log("Mapchange: Kugel-Zentrum Actor erstellen");
kugelDings = $AM.createEmptyActor( "Kugel-Zentrum" );
$SCRIPT.log("Mapchange: Kugel-Zentrum Actor in die Szene einfügen");
kugelDings.placeIntoScene( [-56.2, -1.1, -75.0] );

require 'techdemo2_mapchange.rb'
#Erstellen mit 
mapchangeListener = Td2LevelwechselListener.new("techdemo2map2_01.scene", "Techdemo2", "techdemo2map2.rb"); 

$SCRIPT.log("Mapchange: MapchangeListener hinzufügen");
$GameEveMgr.addSphereAreaListener( kugelDings, 10.000, mapchangeListener, RlScript::QUERYFLAG_PLAYER );


$SCRIPT.log("map 'Techdemo2' initialisiert.");
$WF.toggleCharacterStateWindow()
