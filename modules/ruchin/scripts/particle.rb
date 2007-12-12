

tavernfire1 = $AM.createParticleSystemActor("tavernfire1", "feuertest2" );
tavernfire1.placeIntoScene( 36.74,-3.90,35.93, 1.0, 0.0, 0.0, 0.0 );

templefire1 = $AM.createParticleSystemActor("templefire1", "feuertest2" );
templefire1.placeIntoScene( 123.28,3.0,-22.19, 1.0, 0.0, 0.0, 0.0 );

templefire2 = $AM.createParticleSystemActor("templefire2", "feuertest2" );
templefire2.placeIntoScene( 123.28,3.0,-18.93, 1.0, 0.0, 0.0, 0.0 );

#nebel = $AM.createParticleSystemActor("nebel", "nebel" );
#nebel.placeIntoScene( 32.02, -2.26, -24.40, 1.0, 0.0, 0.0, 0.0 );

#nebel2 = $AM.createParticleSystemActor("nebel2", "nebel" );
#nebel2.placeIntoScene( 26.47, -2.5, -37.92, 1.0, 0.0, 0.0, 0.0 );

qualm1 = $AM.createParticleSystemActor("qualm1", "qualm" );
qualm1.placeIntoScene( -4.28, 3.00, -1.66, 1.0, 0.0, 0.0, 0.0 );

qualm2 = $AM.createParticleSystemActor("qualm2", "qualm" );
qualm2.placeIntoScene( -3.10, 2.18, 6.12, 1.0, 0.0, 0.0, 0.0 );

qualm3 = $AM.createParticleSystemActor("qualm3", "qualm" );
qualm3.placeIntoScene( 34.31, 10.60, -4.16, 1.0, 0.0, 0.0, 0.0 );

qualm4 = $AM.createParticleSystemActor("qualm4", "qualm" );
qualm4.placeIntoScene( 0.42, 3.78, 22.26, 1.0, 0.0, 0.0, 0.0 );

qualm5 = $AM.createParticleSystemActor("qualm5", "qualm" );
qualm5.placeIntoScene( 92.39, 18.35, -30.55, 1.0, 0.0, 0.0, 0.0 );



#rain = $AM.createParticleSystemActor("rain", "rain" );
#rain.placeIntoScene( 0.39, 0.35, 0.55, 1.0, 0.0, 0.0, 0.0 );


#firelight = $AM.createLightActor("firelight", LightObject::LT_POINT);
#firelight.placeIntoScene( 94.28, 5.65, -30.88, 1.0, 0.0, 0.0, 0.0 );
#firelight.getControlledObject().setCastShadows(false);

#firelight.getControlledObject().setAttenuation(10,1,1,1);

#halter= $hero.getActor();
#halter.attachToSlotAxisRot( rain, "l_oberarm", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 0.0, 0.0, 0.0 ], 0.0 );

#halter.attachToSlotAxisRot( firelight, "l_oberarm", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 0.0, 0.0, 0.0 ], 0.0 );
