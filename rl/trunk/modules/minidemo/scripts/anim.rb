# Nur eingebettet starten
load( "embed.rb" );

# print wird auf die Konsole umgeleitet
print( "Teste Animationen" );

# TischAktor erstellen
$Fackel = $AF.createGameActor("Fackel","fackel.mesh");
$Fackel.setPosition(100.0,16.0,100.0);

$boBotter = $AF.createGameActor("Botter","robot.mesh");
$boBotter.setPosition(160.0,24.0,160.0);
$boBotter.startAnimation( "Walk" );

$anim = $boBotter.getAnimation( "Walk" );
$anim.setSpeed( 4.0 );

$trackAnim = $AniMgr.createTrackAnimation( $Fackel, "test", 16.0 );
$trackAnim.addKeyFrame( 0.0 );
$trackAnim.setKeyFrameRotation( 0.0, 0.0, 0.0, 0.0, 0.0 );
$trackAnim.setKeyFrameTranslation( 0.0, 0.0, 0.0, 0.0 );
$trackAnim.addKeyFrame( 8.0 );
$trackAnim.setKeyFrameRotation( 8.0, 1.0, 0.0, 1.0, 90.0 );
$trackAnim.setKeyFrameTranslation( 8.0, 100.0, 0.0, 0.0 );
$trackAnim.addKeyFrame( 16.0 );
$trackAnim.setKeyFrameRotation( 16.0, 0.0, 0.0, 0.0, 0.0 );
$trackAnim.setKeyFrameTranslation( 16.0, 0.0, 0.0, 0.0 );
$trackAnim.setSpeed( 10.0 );