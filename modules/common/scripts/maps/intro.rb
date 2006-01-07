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

greifvogel = $AM.createMeshActor("Greifvogel", "tie_greifvogel.mesh" );
greifvogel.placeIntoScene(0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0);

flugAnim = greifvogel.getControlledObject().startAnimation( "fliegen" );
flugAnim.setSpeed( 1.25 );

trackAnim = $AnimMgr.createTrackAnimation( greifvogel, "Patrouilleflug", 6.0 );
trackAnim.addKeyFrame( 0.0 );
trackAnim.setKeyFrameTranslation( 0.0, -1003.609657, 1218.200302, -1748.200226 );
trackAnim.addKeyFrame( 1.0 );
trackAnim.setKeyFrameTranslation( 1.0, 308.505869, 2025.336266, -4303.391266 );
trackAnim.setKeyFrameRotationQuaternion( 1.0, 0.710575, 0.026213, -0.701152, 0.052746 );
trackAnim.addKeyFrame( 2.0 );
trackAnim.setKeyFrameTranslation( 2.0, 1560.979843, 1134.342003, -2682.585526 );
trackAnim.setKeyFrameRotationQuaternion( 2.0, 0.250410, 0.032495, 0.966347, -0.049125 );
trackAnim.addKeyFrame( 4.0 );
trackAnim.setKeyFrameTranslation( 4.0, -1003.609657, 1218.200302, -1748.200226 );
trackAnim.setKeyFrameRotationQuaternion( 4.0, 0.706683, 0.000000, 0.707531, 0.000000 );
trackAnim.setInterpolationMode( AnimationManager::IM_SPLINE );

trackAnim.setSpeed( 0.25 );
trackAnim.setPaused( false );