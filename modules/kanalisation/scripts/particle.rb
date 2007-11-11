

tavernfire1 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire1.placeIntoScene( -0.75,5.70,12.84, 0.5, 0.0, 0.0, 0.0 );

tavernfire2 = $AM.createParticleSystemActor("tavernfire1", "feuertest" );
tavernfire2.placeIntoScene( 10.73,5.73,10.06, 1.0, 0.0, 0.0, 0.0 );

#firelight = $AM.createLightActor("firelight", LightObject::LT_POINT);
#firelight.placeIntoScene( 94.28, 5.65, -30.88, 1.0, 0.0, 0.0, 0.0 );
#firelight.getControlledObject().setCastShadows(false);

#firelight.getControlledObject().setAttenuation(10,1,1,1);

#halter= $hero.getActor();
#halter.attachToSlotAxisRot( rain, "l_oberarm", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 0.0, 0.0, 0.0 ], 0.0 );

#halter.attachToSlotAxisRot( firelight, "l_oberarm", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 0.0, 0.0, 0.0 ], 0.0 );
