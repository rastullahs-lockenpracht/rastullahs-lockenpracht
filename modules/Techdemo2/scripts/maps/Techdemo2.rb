load "embed.rb"

require 'player.rb'
require 'hero.rb'

# Physik aktivieren
$PM.setEnabled(true);

$SCRIPT.log("init map 'Techdemo2'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "rl/dsa07", 100.0)
$SCRIPT.log("skybox set");

$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(-0.17101, -0.873647, 0.969846);
sunlight.getControlledObject().setCastShadows(true);
sunlight.getControlledObject().setDiffuseColour(0.9,0.9,0.8);
$SCRIPT.log("Tageslicht erstellt.");

#shadowlight = $AM.createLightActor("shadowlight", LightObject::LT_SPOTLIGHT);
#shadowlight.setPosition(0.0, 1000.0, 0.0);
#shadowlight.getControlledObject().setDirection(-0.17101, -0.873647, 0.969846);
#shadowlight.getControlledObject().setCastShadows(true);
#shadowlight.getControlledObject().setDiffuseColour(0.0,0.0,0.0);


$SCRIPT.log("Held erstellen");
$hero = Hero.new;
$SCRIPT.log("Held erstellt");
$SCRIPT.log("Held in die Szene einfuegen.");
$hero.getActor().placeIntoScene(0.0, 0.0, 13.0, 1.0, 0.0, 0.0, 0.0);
$SCRIPT.log("Held eingefügt.");
PlayerSettings.preparePlayer($hero);
$SCRIPT.log("Held vorbereitet.");
$UI.setActiveCharacter($hero);
$SCRIPT.log("Held als aktiver Charakter gesetzt.");

$World.setFog( World::FOG_EXP, [0.8,0.8,1.0,0.5], 0.00103, 0.4, 1.0);

#******** mapchange **********
$SCRIPT.log("Mapchange: Kugel-Zentrum Actor erstellen");
kugelDings = $AM.createEmptyActor( "Kugel-Zentrum" );
$SCRIPT.log("Mapchange: Kugel-Zentrum Actor in die Szene einfügen");
kugelDings.placeIntoScene( [-56.2, -1.1, -75.0] );



class LevelwechselListener < GameAreaListener
	def initialize( targetscene, targetrbfile )
		super()
		@targetScene = targetscene
		@targetRbFile = targetrbfile 
	end
 
	def areaLeft(anEvent)
	end
 
	# Zone betreten
	def areaEntered(anEvent)
		$CORE.loadMap("Octree", @targetScene, @targetRbFile, @targetRbFile); 
	end
end
 
#Erstellen mit 
MapchangeListener = LevelwechselListener .new("minidemo.scene", "minidemo.rb" ); 

$SCRIPT.log("Mapchange: MapchangeListener hinzufügen");
$GameEveMgr.addSphereAreaListener( kugelDings, 4.000, MapchangeListener, Actor::QGF_PLAYER );









$SCRIPT.log("map 'Techdemo2' initialisiert.");
$WF.toggleCharacterStateWindow()
