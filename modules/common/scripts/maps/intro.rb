$CORE.log("init map 'minidemo'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "rl/dsa07")
$CORE.log("skybox set");

$CORE.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(0.8, -1.5, 3.0);
sunlight.getControlledObject().setCastShadows(false);
sunlight.getControlledObject().setDiffuseColour(0.9,0.9,0.8);
$CORE.log("Tageslicht erstellt.");


$CORE.log("Sound laden");
titelLied = $AM.createSoundSampleActor( "TitelLied", "spannend.ogg" );
$CORE.log(" Platzieren");
titelLied.placeIntoScene( 160.0, 24.0, 160.0, 1.0, 0.0, 0.0, 0.0 );
$CORE.log(" Loopen");
titelLied.getControlledObject().setLooping( true );
$CORE.log(" Abspielen");
titelLied.getControlledObject().play();
$CORE.log("Sound fertig");