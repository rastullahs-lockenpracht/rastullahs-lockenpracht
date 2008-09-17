load "embed.rb"

require 'player.rb'
require 'hero.rb'

# Physik aktivieren
$PM.setEnabled(true);

$SCRIPT.log("Held erstellen");
$hero = $GOM.createGameObject("hero", 1);
$hero.placeIntoScene()
PlayerSettings.preparePlayer($hero)
PartyManager.getSingleton().setActiveCharacter($hero)
$SCRIPT.log("Held erstellt");

#$heroVehicle = $AGM.createAgent(AgentManager::AGENT_PLAYER, $hero);	
#$SCRIPT.log("SteeringVehicle für Held erstellen...");


$SCRIPT.log("init map 'Techdemo2'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "rl/desert07", 100.0)
$SCRIPT.log("skybox set");

$SCRIPT.log("Weiler erstellen..");
weiler = $AM.createMeshActor("weiler", "nat_wasser_techdemo2.mesh", PhysicsManager::GT_NONE, 0.0);
weiler.placeIntoScene([25.0, -79.0, -138.0]);
$SCRIPT.log("Weiler erstellt");

$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(-0.17101, -0.873647, 0.969846);
sunlight.getControlledObject().setCastShadows(true);
sunlight.getControlledObject().setDiffuseColour(0.8,0.8,0.8);
$SCRIPT.log("Tageslicht erstellt.");


#shadowlight = $AM.createLightActor("shadowlight", LightObject::LT_SPOTLIGHT);
#shadowlight.setPosition(0.0, 1000.0, 0.0);
#shadowlight.getControlledObject().setDirection(-0.17101, -0.873647, 0.969846);
#shadowlight.getControlledObject().setCastShadows(true);
#shadowlight.getControlledObject().setDiffuseColour(0.0,0.0,0.0);


$SCRIPT.log("Druidenwald-Thema laden");
techdemoDruidenWald = $AM.createSoundSampleActor( "techdemoDruidenWald", "techdemo002.ogg" ); 
techdemoDruidenWald.getControlledObject().set3d(false);
$SCRIPT.log(" Loopen");
techdemoDruidenWald.getControlledObject().setLooping( true );
$SCRIPT.log(" Laden");
techdemoDruidenWald.getControlledObject().load();
$SCRIPT.log("Druidenwald-Thema fertig");

$SCRIPT.log("Ruchins Ruche -Thema laden");
ruchinsRuhe = $AM.createSoundSampleActor( "ruchinsRuhe", "ruchin002.ogg" ); 
ruchinsRuhe.getControlledObject().set3d(false);
$SCRIPT.log(" Loopen");
ruchinsRuhe.getControlledObject().setLooping( true );
$SCRIPT.log(" Laden");
ruchinsRuhe.getControlledObject().load();
$SCRIPT.log("Ruchins Ruche -Thema fertig");

$SCRIPT.log("Kampf -Thema laden");
kampfMusik = $AM.createSoundSampleActor( "kampfMusik", "ruchin004.ogg" ); 
kampfMusik.getControlledObject().set3d(false);
$SCRIPT.log(" Loopen");
kampfMusik.getControlledObject().setLooping( true );
$SCRIPT.log(" Laden");
kampfMusik.getControlledObject().load();
$SCRIPT.log("Kampf -Thema fertig");

#Zu laut, da stimmt inzwischen was nicht. 
$SCRIPT.log("Wind laden");
wind = $AM.createSoundSampleActor( "wind", "wind_hintergrund_001.ogg" ); 
wind.getControlledObject().set3d(false);
$SCRIPT.log(" Loopen");
wind.getControlledObject().setLooping( true );
$SCRIPT.log(" Abspielen");
#wind.getControlledObject().play();
$SCRIPT.log(" Volumen");
wind.getControlledObject().setVolume( 0.15 )

windBoee = $AM.createSoundSampleActor( "windBoee", "wind_boee_001.ogg" ); 
windBoee.getControlledObject().set3d(true);
$SCRIPT.log(" Loopen");
windBoee.getControlledObject().setLooping( true );
$SCRIPT.log(" Setzen");
windBoee.placeIntoScene([12.97, -58.20, -220.37]);
$SCRIPT.log(" Abspielen");
#windBoee.getControlledObject().play();
$SCRIPT.log(" Volumen");
windBoee.getControlledObject().setVolume( 1.0 )

windBaum2 = $AM.createSoundSampleActor( "windBaum2", "wind_baumrascheln_001.ogg" ); 
windBaum2.getControlledObject().set3d(true);
$SCRIPT.log(" Loopen");
windBaum2.getControlledObject().setLooping( true );
$SCRIPT.log(" Setzen");
windBaum2.placeIntoScene([-12.51, -65.07, -89.64]);
$SCRIPT.log(" Abspielen");
#windBaum2.getControlledObject().play();
$SCRIPT.log(" Volumen");
windBaum2.getControlledObject().setVolume( 0.7 )

windBaum = $AM.createSoundSampleActor( "windBaum", "wind_baumrascheln_001.ogg" ); 
windBaum.getControlledObject().set3d(true);
$SCRIPT.log(" Loopen");
windBaum.getControlledObject().setLooping( true );
$SCRIPT.log(" Setzen");
windBaum.placeIntoScene([-17.36, -73.56, -124.78]);
$SCRIPT.log(" Abspielen");
#windBaum.getControlledObject().play();
$SCRIPT.log(" Volumen");
windBaum.getControlledObject().setVolume( 0.7 )
$SCRIPT.log("Wind fertig");



$World.setFog( World::FOG_EXP, [0.1,0.08,0.01,0.1], 0.00503, 10.0, 100.0);

#nur ein Test! Ist mit Mapwechsel nicht mehr nötig
#$SCRIPT.log("IN DER MAPWECHSEL VERSION RAUSNEHMEN!");
#load("techdemo2_quests.rb");
#Test zuende


$SCRIPT.log("SteeringBehaviours laden");
load("steering.rb");

if (RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest41").getState() == Quest::ASSIGNED)
	# Tagebuch
	entry = $tagebuchtexte['schnetzeln0']
	RulesSubsystem.getSingleton().getQuestBook().addJournalEntry( entry[0], entry[1] );
	load( "schnetzeln.rb" );
	#Steinschlag
	@mSteinschlagSound = $AM.createSoundSampleActor("Steinschlag","steinschlag_wenig_zu_vielen.ogg");
	@mSteinschlagSound.getControlledObject().set3d(false);
	@mSteinschlagSound.getControlledObject().load();
end

if (RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest42").getState() == Quest::ASSIGNED)
	# Tagebuch
	entry = $tagebuchtexte['retten0']
	RulesSubsystem.getSingleton().getQuestBook().addJournalEntry( entry[0], entry[1] );
	load( "retten.rb" );
end

#Musik als letztes abspielen
techdemoDruidenWald.getControlledObject().play();


#******** mapchange **********
$SCRIPT.log("Mapchange: Kugel-Zentrum Actor erstellen");
kugelDings = $AM.createEmptyActor( "Kugel-Zentrum" );
$SCRIPT.log("Mapchange: Kugel-Zentrum Actor in die Szene einfügen");
kugelDings.placeIntoScene( [ 23.7184276580811, -36.9368743896484, 7.94406747817993] );



class LevelwechselListener < GameAreaListener
	def initialize( targetscene, resourcegrp, targetrbfile )
		super()
		@targetScene = targetscene
		@targetRbFile = targetrbfile
		@resourcegrp = resourcegrp 
	end
 
	def areaLeft(anEvent)
	end
 
	# Zone betreten
	def areaEntered(anEvent)
		#Kein mapchange zurück!
		$WF.showMessageWindow( "Ihr müsst eure Gruppe erst sammeln, bevor Ihr das Gebiet verlassen könnt." );
	end
end
 
#Erstellen mit 
MapchangeListener = LevelwechselListener.new("techdemo2map1_02.scene", "Techdemo2", "techdemo2map1.rb"); 

$SCRIPT.log("Mapchange: MapchangeListener hinzufügen");
$GameEveMgr.addSphereAreaListener( kugelDings, 5.000, MapchangeListener, RlScript::QUERYFLAG_PLAYER );

$SCRIPT.log("map 'Techdemo2' initialisiert.");
$WF.toggleCharacterStateWindow()
