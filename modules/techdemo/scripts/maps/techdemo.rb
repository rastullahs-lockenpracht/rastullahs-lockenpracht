load "embed.rb"

# Physik aktivieren
$PM.setEnabled(true);

$SCRIPT.log("init map 'techdemo'...");
$World = $CORE.getWorld()

$SCRIPT.log("Welten-Thema laden");
techdemoWelt = $AM.createSoundSampleActor( "techdemoWelt", "techdemo001.ogg" ); 
techdemoWelt.getControlledObject().set3d(false);
$SCRIPT.log(" Loopen");
techdemoWelt.getControlledObject().setLooping( true );
$SCRIPT.log(" Abspielen");
techdemoWelt.getControlledObject().play();
$SCRIPT.log("Welten-Thema fertig");

$SCRIPT.log("Get hero")
hero = $GOM.getGameObject(1);

$SCRIPT.log("Prepare hero for being the active character")
PlayerSettings.preparePlayer(hero)

itemToAdd = $GOM.createGameObject("torch");
hero.getInventory().hold(itemToAdd, "Right Hand");


$SCRIPT.log("Set hero as active character")
PartyManager.getSingleton().setActiveCharacter(hero)

$World.setFog( World::FOG_EXP, [0.8,0.8,1.0,0.5], 0.00003, 0.4, 1.0);

tueroeffner = CaveDoorOpener.new(
	$GOM.getGameObject(7), 
	$GOM.getGameObject(8))

truhe = $GOM.getGameObject(9);
truhe.addItem($GOM.getGameObject(10))


luftfels1 = $GOM.getGameObject(11)
luftfels2 = $GOM.getGameObject(12)
luftfels3 = $GOM.getGameObject(13)

$SCRIPT.log("Steinschlag: SteinschlagzoneListener erstellen");
areaListener = SteinschlagzoneListener.new(luftfels1, luftfels2, luftfels3 );

felshaufen = $GOM.getGameObject(14)
felshaufen.getActor().getPhysicalThing().setContactListener( 
	RockpileContactListener.new($felshaufen, luftfels1, luftfels2, luftfels3) );


$SCRIPT.log("Steinschlag: Kugel-Zentrum Actor erstellen");
kugelDings = $AM.createEmptyActor( "Kugel-Zentrum" );
$SCRIPT.log("Steinschlag: Kugel-Zentrum Actor in die Szene einfügen");
kugelDings.placeIntoScene( [78.1769, 10.1317, 50.9391] );



$SCRIPT.log("Steinschlag: SteinschlagzoneListener hinzufügen");
$GameEveMgr.addSphereAreaListener( kugelDings, 4.000, areaListener, RlScript::QUERYFLAG_PLAYER );

$SCRIPT.log("map 'techdemo' initialisiert.");
$WF.toggleCharacterStateWindow()
