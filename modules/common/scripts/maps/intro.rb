$SCRIPT.log("init map 'minidemo'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "himmel/wueste")
$SCRIPT.log("skybox set");

$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(0.7, -2.0, 0.8);
sunlight.getControlledObject().setCastShadows(false);
sunlight.getControlledObject().setDiffuseColour(1.0,1.0,0.9);
$SCRIPT.log("Tageslicht erstellt.");


$World.setFog( World::FOG_EXP, [0.7,0.6,0.5,0.5], 0.00009, 0.5, 1.0);


$SCRIPT.log("Sound laden");
titelLied = $AM.createSoundSampleActor( "TitelLied", "ruchin001.ogg" ); 
$SCRIPT.log(" Platzieren");
titelLied.placeIntoScene( 160.0, 24.0, 160.0, 1.0, 0.0, 0.0, 0.0 );
$SCRIPT.log(" Loopen");
titelLied.getControlledObject().setLooping( true );
$SCRIPT.log(" Abspielen");
titelLied.getControlledObject().play();
$SCRIPT.log("Sound fertig");


sandsturm = $AM.createParticleSystemActor("sandsturm_1", "Rl/Sandsturm" );
sandsturm.placeIntoScene( -125.482821, 99.060595, -2469.191551, 1.0, 0.0, 0.0, 0.0 );
sandsturm = $AM.createParticleSystemActor("sandsturm_2", "Rl/Sandsturm" );
sandsturm.placeIntoScene( 111.617033, -87.779894, -350.628781, 1.0, 0.0, 0.0, 0.0 );
held = $AM.createMeshActor("KreisLaufHeld", "held.mesh" );
held.placeIntoScene( -1125.412178, 742.069721, -1868.909645, 0.976916, 0.061914, 0.204045, -0.012932 );
