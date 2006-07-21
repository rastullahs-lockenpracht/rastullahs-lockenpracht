load "embed.rb"

require 'player.rb'
require 'hero.rb'

# Physik aktivieren
$PM.setEnabled(true);

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
$SCRIPT.log(" Abspielen");
techdemoDruidenWald.getControlledObject().play();
$SCRIPT.log("Druidenwald-Thema fertig");

$SCRIPT.log("Ruchins Ruche -Thema laden");
ruchinsRuhe = $AM.createSoundSampleActor( "ruchinsRuhe", "ruchin002.ogg" ); 
ruchinsRuhe.getControlledObject().set3d(false);
$SCRIPT.log(" Loopen");
ruchinsRuhe.getControlledObject().setLooping( true );
$SCRIPT.log(" Abspielen");
ruchinsRuhe.getControlledObject().load();
$SCRIPT.log("Ruchins Ruche -Thema fertig");

$SCRIPT.log("Kampf -Thema laden");
kampfMusik = $AM.createSoundSampleActor( "kampfMusik", "ruchin004.ogg" ); 
kampfMusik.getControlledObject().set3d(false);
$SCRIPT.log(" Loopen");
kampfMusik.getControlledObject().setLooping( true );
$SCRIPT.log(" Abspielen");
kampfMusik.getControlledObject().load();
$SCRIPT.log("Kampf -Thema fertig");


$SCRIPT.log("Held erstellen");
$hero = Hero.new;
$SCRIPT.log("Held erstellt");
$SCRIPT.log("Held in die Szene einfuegen.");
$hero.getActor().placeIntoScene(0.99, -36.95, -7.68, 1.0, 0.0, 0.0, 0.0);
$SCRIPT.log("Held eingefügt.");
PlayerSettings.preparePlayer($hero);
$SCRIPT.log("Held vorbereitet.");
$UI.setActiveCharacter($hero);
$SCRIPT.log("Held als aktiver Charakter gesetzt.");

$World.setFog( World::FOG_EXP, [0.1,0.08,0.01,0.1], 0.00503, 10.0, 100.0);

#nur ein Test! Ist mit Mapwechsel nicht mehr nötig
$SCRIPT.log("IN DER MAPWECHSEL VERSION RAUSNEHMEN!");
load("techdemo2_quests.rb");
#Test zuende


$SCRIPT.log("SteeringBehaviours laden");
load("steering.rb");

#if (RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest41").getState() == Quest::ASSIGNED)
	load( "schnetzeln.rb" );
	#Steinschlag
	@mSteinschlagSound = $AM.createSoundSampleActor("Steinschlag","steinschlag_wenig_zu_vielen.ogg");
	@mSteinschlagSound.getControlledObject().set3d(false);
	@mSteinschlagSound.getControlledObject().load();


#if (RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest42").getState() == Quest::ASSIGNED)
	#load( "retten.rb" );



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
		$WF.showMessageWindow( "Eure Aufgababe hier ist noch nicht beendet. Ihr müsst eure Gruppe erst sammeln, bevor Ihr das Gebiet verlassen könnt." );
	end
end
 
#Erstellen mit 
MapchangeListener = LevelwechselListener .new("techdemo2map1_02.scene", "Techdemo2", "Techdemo2map1.rb"); 

$SCRIPT.log("Mapchange: MapchangeListener hinzufügen");
$GameEveMgr.addSphereAreaListener( kugelDings, 5.000, MapchangeListener, Actor::QGF_PLAYER );

$SCRIPT.log("map 'Techdemo2' initialisiert.");
$WF.toggleCharacterStateWindow()