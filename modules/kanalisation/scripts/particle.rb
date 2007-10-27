

tavernfire1 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire1.placeIntoScene( 22.44,6.75,8.00, 1.0, 0.0, 0.0, 0.0 );

tavernfire2 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire2.placeIntoScene( 22.44,6.75,15.80, 1.0, 0.0, 0.0, 0.0 );

tavernfire3 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire3.placeIntoScene( -10.16,6.47,17.10, 1.0, 0.0, 0.0, 0.0 );

tavernfire4 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire4.placeIntoScene( -9.14,6.53,2.80, 1.0, 0.0, 0.0, 0.0 );

tavernfire5 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire5.placeIntoScene( -9.14,6.53,-4.74, 1.0, 0.0, 0.0, 0.0 );

tavernfire6 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire6.placeIntoScene( 22.53,6.70,25.60, 1.0, 0.0, 0.0, 0.0 );

#firelight = $AM.createLightActor("firelight", LightObject::LT_POINT);
#firelight.placeIntoScene( 94.28, 5.65, -30.88, 1.0, 0.0, 0.0, 0.0 );
#firelight.getControlledObject().setCastShadows(false);

#firelight.getControlledObject().setAttenuation(10,1,1,1);

#halter= $hero.getActor();
#halter.attachToSlotAxisRot( rain, "l_oberarm", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 0.0, 0.0, 0.0 ], 0.0 );

#halter.attachToSlotAxisRot( firelight, "l_oberarm", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 0.0, 0.0, 0.0 ], 0.0 );
