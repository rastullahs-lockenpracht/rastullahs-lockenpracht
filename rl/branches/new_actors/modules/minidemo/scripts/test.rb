# Nur eingebettet starten
load( "embed.rb" );

# print wird auf die Konsole umgeleitet
print( "Test-Script" );

# Spiel Akteur erstellen
$AF.createGameActor("Tisch","tisch_taverne.mesh");
$gTisch = $AF.getActor( "Tisch" );

# und positionieren
$gTisch.setPosition(-128.0,128.0,16.0);
$gTisch.setScale(0.5,0.5,0.5);

#$gTisch.addSound("testsound.wav");
#$gTisch.playSound("testsound.wav");

#$gTrackController = TrackController.new( $gTisch );

#$gTrackController.createAnimation("Test",10);

#$gTrackController.addAnimationKeyFrame(  0.0 );
#$gTrackController.setTranslateKeyFrame(  0.0, 0.0, 0.0,  0.0 );

#$gTrackController.addAnimationKeyFrame(  5.0 );
#$gTrackController.setTranslateKeyFrame(  5.0, 0.0,   128.0,  256.0 );
#$gTrackController.setRotateKeyFrame(  5.0, 0.0,     0.0,    1.0,  90.0 );

#$gTrackController.addAnimationKeyFrame( 10.0 );
#$gTrackController.setTranslateKeyFrame( 10.0, 0.0, 0.0,  0.0 );


#$gTrackController.setInterpolationMode(TrackController::IM_SPLINE);
#$gTrackController.startAnimation();

neKugel = $AF.createGameActor("neKugel","sphere.mesh");
neKugel.setPosition(-128.0,128.0,24.0);
neKugel.setScale(0.1,0.1,0.1);


Licht = $AF.createLightActor("EinsamesLeuchten");
Licht.setPosition(-120.0,128.0,180.0);


Purpels = $AF.createParticleSystemActor("purpleHill","Examples/PurpleFountain");
Purpels.setPosition(-128.0,128.0,24.0);

$boBotter = $AF.createGameActor("boBotter","robot.mesh");
$boBotter.setPosition(-128.0,28.0,24.0);
$boBotter.startAnimation( "Walk" );
