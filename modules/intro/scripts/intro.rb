$SCRIPT.log("init map 'minidemo'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "himmel/wueste", 100.0)
$SCRIPT.log("skybox set");

$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(0.7, -2.0, 0.8);
#sunlight.getControlledObject().setCastShadows(false);
sunlight.getControlledObject().setDiffuseColour(1.0,1.0,0.9);
$SCRIPT.log("Tageslicht erstellt.");

$IM.pushControlState(InputManager::CST_CUTSCENE);

$World.setFog( World::FOG_EXP, [0.7,0.6,0.5,0.5], 0.00009, 0.5, 1.0);


$SCRIPT.log("Sound laden");
titelLied = $AM.createSoundStreamActor( "TitelLied", "ruchin001.ogg" ); 
titelLied.getControlledObject().set3d(false);
$SCRIPT.log(" Loopen");
titelLied.getControlledObject().setLooping( true );
$SCRIPT.log(" Abspielen");
titelLied.getControlledObject().play();
$SCRIPT.log("Sound fertig");


sandsturm = $AM.createParticleSystemActor("sandsturm_1", "Rl/Sandsturm" );
sandsturm.placeIntoScene( -1.25482821, 0.99060595, -24.69191551, 1.0, 0.0, 0.0, 0.0 );
sandsturm = $AM.createParticleSystemActor("sandsturm_2", "Rl/Sandsturm" );
sandsturm.placeIntoScene( 1.11617033, -0.87779894, -3.50628781, 1.0, 0.0, 0.0, 0.0 );
held = $AM.createMeshActor("Alrike", "men_alrike.mesh" );
held.getControlledObject().startAnimation("idle");
held.placeIntoScene( -11.25412178, 7.42069721, -18.68909645, 0.976916, 0.061914, 0.204045, -0.012932 );

greifvogel = $AM.createMeshActor("Greifvogel", "tie_greifvogel.mesh" );
greifvogel.placeIntoScene(0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0);

flugAnim = greifvogel.getControlledObject().startAnimation( "fliegen" );
flugAnim.setSpeed( 1.25 );

trackAnim = $AnimMgr.createTrackAnimation( greifvogel, "Patrouilleflug", 6.0 );
trackAnim.addKeyFrame( 0.0 );
trackAnim.setKeyFrameTranslation( 0.0, -10.03609657, 12.18200302, -17.48200226 );
trackAnim.addKeyFrame( 1.0 );
trackAnim.setKeyFrameTranslation( 1.0, 3.08505869, 20.025336266, -43.03391266 );
trackAnim.setKeyFrameRotationQuaternion( 1.0, 0.710575, 0.026213, -0.701152, 0.052746 );
trackAnim.addKeyFrame( 2.0 );
trackAnim.setKeyFrameTranslation( 2.0, 15.60979843, 11.34342003, -26.82585526 );
trackAnim.setKeyFrameRotationQuaternion( 2.0, 0.250410, 0.032495, 0.966347, -0.049125 );
trackAnim.addKeyFrame( 4.0 );
trackAnim.setKeyFrameTranslation( 4.0, -10.03609657, 12.18200302, -17.48200226 );
trackAnim.setKeyFrameRotationQuaternion( 4.0, 0.706683, 0.000000, 0.707531, 0.000000 );
trackAnim.setInterpolationMode( AnimationManager::IM_SPLINE );
trackAnim.setDelay( 1.0 );
trackAnim.setSpeed( 0.25 );
trackAnim.setPaused( false );
