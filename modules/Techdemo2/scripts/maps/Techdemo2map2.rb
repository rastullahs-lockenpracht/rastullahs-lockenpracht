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


#[ 23.7184276580811, -36.9368743896484, 7.94406747817993]

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

$SCRIPT.log("Held erstellen");
$hero = Hero.new;
$SCRIPT.log("Held erstellt");
$SCRIPT.log("Held in die Szene einfuegen.");
$hero.getActor().placeIntoScene(9.0, -77.0, -147.0, 1.0, 0.0, 0.0, 0.0);
$SCRIPT.log("Held eingefügt.");
PlayerSettings.preparePlayer($hero);
$SCRIPT.log("Held vorbereitet.");
$UI.setActiveCharacter($hero);
$SCRIPT.log("Held als aktiver Charakter gesetzt.");

$World.setFog( World::FOG_EXP, [0.1,0.08,0.01,0.1], 0.00503, 10.0, 100.0);

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
		#if (RulesSubsystem.getSingleton().getQuestBook().getQuest("aufenthaltsort").getState() == Quest::SUCCEEDED) &&  ( (RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest42").getState() == Quest::SUCCEEDED)  || (RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest41").getState() == Quest::SUCCEEDED) )
			$CORE.loadMap("Octree", @targetScene, @resourcegrp, @targetRbFile); 
		#else
		#	$WF.showMessageWindow( "Ihr wisst noch nicht wohin ihr wollt - und was ihr an diesem Ort suchen wollt. Findet es erst heraus!" );
	
		#end
	end
end
 
#Erstellen mit 
MapchangeListener = LevelwechselListener .new("techdemo2map1_02.scene", "Techdemo2", "Techdemo2map1.rb"); 

$SCRIPT.log("Mapchange: MapchangeListener hinzufügen");
$GameEveMgr.addSphereAreaListener( kugelDings, 5.000, MapchangeListener, Actor::QGF_PLAYER );

$SCRIPT.log("map 'Techdemo2' initialisiert.");
$WF.toggleCharacterStateWindow()
