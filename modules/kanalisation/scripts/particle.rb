

tavernfire1 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire1.placeIntoScene( -3.77,5.71,13.40, 1.0, 0.0, 0.0, 0.0 );

tavernfire2 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire2.placeIntoScene( -3.24,5.74,5.43, 1.0, 0.0, 0.0, 0.0 );

tavernfire3 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire3.placeIntoScene( -3.24,5.74,1.21, 1.0, 0.0, 0.0, 0.0 );

tavernfire4 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire4.placeIntoScene( 14.48,5.84,12.72, 1.0, 0.0, 0.0, 0.0 );

tavernfire5 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire5.placeIntoScene( 14.46,5.81,18.23, 1.0, 0.0, 0.0, 0.0 );

tavernfire6 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire6.placeIntoScene( 14.49,5.83,8.39, 1.0, 0.0, 0.0, 0.0 );

#firelight = $AM.createLightActor("firelight", LightObject::LT_POINT);
#firelight.placeIntoScene( 94.28, 5.65, -30.88, 1.0, 0.0, 0.0, 0.0 );
#firelight.getControlledObject().setCastShadows(false);

#firelight.getControlledObject().setAttenuation(10,1,1,1);

#halter= $hero.getActor();
#halter.attachToSlotAxisRot( rain, "l_oberarm", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 0.0, 0.0, 0.0 ], 0.0 );

#halter.attachToSlotAxisRot( firelight, "l_oberarm", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 0.0, 0.0, 0.0 ], 0.0 );
