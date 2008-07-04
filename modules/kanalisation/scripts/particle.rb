tavernfire1 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire1.placeIntoScene( -1.39,5.54,12.84, 0.5, 0.0, 0.0, 0.0 );

tavernfire2 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire2.placeIntoScene( 11.44,5.54,9.76, 1.0, 0.0, 0.0, 0.0 );

tavernfire3 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire3.placeIntoScene( -22.48,4.60,23.10, 1.0, 0.0, 0.0, 0.0 );



tavernfire5 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire5.placeIntoScene( 36.49,7.37,30.27, 1.0, 0.0, 0.0, 0.0 );

tavernfire6 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire6.placeIntoScene( -28.65,4.77,23.94, 1.0, 0.0, 0.0, 0.0 );

tavernfire6 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire6.placeIntoScene( -28.65,4.77,28.39, 1.0, 0.0, 0.0, 0.0 );

#waber01 = $AM.createParticleSystemActor("waber01", "waber" );
#waber01.placeIntoScene( 8.73,3.73,10.36, 1.0, 0.0, 0.0, 0.0 );

#firelight = $AM.createLightActor("firelight", LightObject::LT_POINT);
#firelight.placeIntoScene( 94.28, 5.65, -30.88, 1.0, 0.0, 0.0, 0.0 );
#firelight.getControlledObject().setCastShadows(true);
#firelight.getControlledObject().setAttenuation(4,1,1,1);

#halter= $hero.getActor();
#halter.attachToSlotAxisRot( rain, "l_oberarm", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 0.0, 0.0, 0.0 ], 0.0 );
#halter.attachToSlotAxisRot( firelight, "l_oberarm", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 0.0, 0.0, 0.0 ], 0.0 );
