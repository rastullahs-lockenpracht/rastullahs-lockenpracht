load "embed.rb"

require 'player.rb'
require 'hero.rb'
require 'door.rb'

# Physik aktivieren
$PM.setEnabled(true);

$SCRIPT.log("init map 'Techdemo2'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "rl/desert07", 100.0)
$SCRIPT.log("skybox set");


$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(-0.17101, -0.873647, 0.969846);
sunlight.getControlledObject().setCastShadows(true);
sunlight.getControlledObject().setDiffuseColour(0.8,0.8,0.8);
$SCRIPT.log("Tageslicht erstellt.");


$SCRIPT.log("Türen einsetzen")
#door1 = Door.new("Eine Tür links", "Eine Holztür", [-12.9124, -6.86492, 13.7133], [0, -120, 0], "arc_door_02.mesh");
#door1.addActions(false, true)
#door2 = Door.new("Eine Tür links", "Eine Holztür", [-34.6458, -6.2058, 14.9355], [0, -212.5, 0], "arc_door_02.mesh");
#door2.addActions(false, true)
$SCRIPT.log("Türen gesetzt")

#shadowlight = $AM.createLightActor("shadowlight", LightObject::LT_SPOTLIGHT);
#shadowlight.setPosition(0.0, 1000.0, 0.0);
#shadowlight.getControlledObject().setDirection(-0.17101, -0.873647, 0.969846);
#shadowlight.getControlledObject().setCastShadows(true);
#shadowlight.getControlledObject().setDiffuseColour(0.0,0.0,0.0);

#Nerviges Zeug... -Steele
$SCRIPT.log("Musik vom Komponisten deaktiviert. Bei Bedarf reaktivieren.");
#$SCRIPT.log("Dorf-Thema laden");
#techdemoDruidenWald = $AM.createSoundSampleActor( "techdemoWelt", "techdemo001.ogg" ); 
#techdemoDruidenWald.getControlledObject().set3d(false);
#$SCRIPT.log(" Loopen");
#techdemoDruidenWald.getControlledObject().setLooping( true );
#$SCRIPT.log(" Abspielen");
#techdemoDruidenWald.getControlledObject().play();
#$SCRIPT.log("Dorf-Themafertig");

$SCRIPT.log("Held erstellen");
$hero = Hero.new;
$SCRIPT.log("Held erstellt");
$SCRIPT.log("Held in die Szene einfuegen.");#-22.0130577087402, -6.70670890808105, 25.1438484191895
$hero.getActor().placeIntoScene(-22.0130577087402, -6.70670890808105, 25.1438484191895, 1.0, 0.0, 0.0, 0.0);
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
kugelDings.placeIntoScene( [-56.2, -1.1, -75.0] );



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
		$CORE.loadMap("Octree", @targetScene, @resourcegrp, @targetRbFile); 
	end
end
 
#Erstellen mit 
MapchangeListener = LevelwechselListener .new("techdemo2map2_01.scene", "Techdemo2", "Techdemo2map2.rb"); 

$SCRIPT.log("Mapchange: MapchangeListener hinzufügen");
$GameEveMgr.addSphereAreaListener( kugelDings, 4.000, MapchangeListener, Actor::QGF_PLAYER );


$SCRIPT.log("map 'Techdemo2' initialisiert.");
$WF.toggleCharacterStateWindow()
