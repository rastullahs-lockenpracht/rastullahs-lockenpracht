

tavernfire1 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire1.placeIntoScene( -1.39,5.71,12.84, 0.5, 0.0, 0.0, 0.0 );

tavernfire2 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire2.placeIntoScene( 11.39,5.75,9.75, 1.0, 0.0, 0.0, 0.0 );

waber01 = $AM.createParticleSystemActor("waber01", "waber" );
waber01.placeIntoScene( 8.73,3.73,10.36, 1.0, 0.0, 0.0, 0.0 );

#glow1 = $AM.createParticleSystemActor("glow1", "glow01" );
#glow1.placeIntoScene( 8.49,2.83,10.39, 1.0, 0.0, 0.0, 0.0 );
#firelight = $AM.createLightActor("firelight", LightObject::LT_POINT);
#firelight.placeIntoScene( 94.28, 5.65, -30.88, 1.0, 0.0, 0.0, 0.0 );
#firelight.getControlledObject().setCastShadows(false);

#firelight.getControlledObject().setAttenuation(10,1,1,1);

#halter= $hero.getActor();
#halter.attachToSlotAxisRot( rain, "l_oberarm", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 0.0, 0.0, 0.0 ], 0.0 );

#halter.attachToSlotAxisRot( firelight, "l_oberarm", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 0.0, 0.0, 0.0 ], 0.0 );
