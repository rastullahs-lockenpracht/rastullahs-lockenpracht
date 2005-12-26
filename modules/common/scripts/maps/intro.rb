$SCRIPT.log("init map 'minidemo'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "rl/dsa07")
$SCRIPT.log("skybox set");

$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(0.7, -2.0, 0.8);
sunlight.getControlledObject().setCastShadows(false);
sunlight.getControlledObject().setDiffuseColour(0.9,0.9,0.8);
$SCRIPT.log("Tageslicht erstellt.");


$SCRIPT.log("Sound laden");
titelLied = $AM.createSoundSampleActor( "TitelLied", "ruchin001.ogg" ); 
$SCRIPT.log(" Platzieren");
titelLied.placeIntoScene( 160.0, 24.0, 160.0, 1.0, 0.0, 0.0, 0.0 );
$SCRIPT.log(" Loopen");
titelLied.getControlledObject().setLooping( true );
$SCRIPT.log(" Abspielen");
titelLied.getControlledObject().play();
$SCRIPT.log("Sound fertig");
