# Nur eingebettet starten
load( "embed.rb" );
 
print("Der Wald wird schoen");
#Schmetterlinge

schmetterling1 = $AM.createMeshActor("Schmetterling1", "tie_schmetterling_01.mesh" );
schmetterling1.placeIntoScene([ 10.0053634643555, -70.6423110961914, -176.172500610352]);
schmetterling1.getControlledObject().startAnimation("idle");

schmetterling2 = $AM.createMeshActor("Schmetterling2", "tie_schmetterling_02.mesh" );
schmetterling2.placeIntoScene([ -24.0168075561523, -69.3453140258789, -149.193054199219]);
schmetterling2.setScale( 1.2, 1.2, 1.2 );
schmetter2Anim = schmetterling2.getControlledObject().startAnimation("idle");
schmetter2Anim.setSpeed( 1.1 );

schmetterling3 = $AM.createMeshActor("Schmetterling3", "tie_schmetterling_01.mesh" );
schmetterling3.placeIntoScene([ -25.1370658874512, -67.5108947753906, -119.962600708008]);
schmetter3Anim = schmetterling3.getControlledObject().startAnimation("idle");
schmetter3Anim.setSpeed( 1.25 );

schmetterling4 = $AM.createMeshActor("Schmetterling4", "tie_schmetterling_02.mesh" );
schmetterling4.placeIntoScene([ -1.67383909225464, -67.3966064453125, -103.239326477051]);
schmetter4Anim = schmetterling4.getControlledObject().startAnimation("idle");
schmetter4Anim.setSpeed( 1.3 );

schmetterling5 = $AM.createMeshActor("Schmetterling5", "tie_schmetterling_01.mesh" );
schmetterling5.placeIntoScene([ -17.159948348999, -58.0644111633301, -78.3551025390625]);
schmetterling5.setScale( 1.3, 1.3, 1.3 );
schmetter5Anim = schmetterling5.getControlledObject().startAnimation("idle");
schmetter5Anim.setSpeed( 0.8 );

schmetterling6 = $AM.createMeshActor("Schmetterling6", "tie_schmetterling_01.mesh" );
schmetterling6.placeIntoScene([ -3.85465240478516, -55.8962364196777, -68.6688613891602]);
schmetter6Anim = schmetterling6.getControlledObject().startAnimation("idle");
schmetter6Anim.setSpeed( 1.0 );

schmetterling7 = $AM.createMeshActor("Schmetterling7", "tie_schmetterling_02.mesh" );
schmetterling7.placeIntoScene([ -6.78132486343384, -49.7682075500488, -57.5737953186035]);
schmetter7Anim = schmetterling7.getControlledObject().startAnimation("idle");
schmetter7Anim.setSpeed( 1.25 );

schmetterling8 = $AM.createMeshActor("Schmetterling8", "tie_schmetterling_02.mesh" );
schmetterling8.placeIntoScene([ 4.51901435852051, -43.7882385253906, -45.3944702148438]);
schmetterling5.setScale( 0.7, 0.7, 0.7 );
schmetter8Anim = schmetterling8.getControlledObject().startAnimation("idle");
schmetter8Anim.setSpeed( 2.0 );

schmetterling9 = $AM.createMeshActor("Schmetterling9", "tie_schmetterling_01.mesh" );
schmetterling9.placeIntoScene([ -3.94496154785156, -38.1320869445801, -34.6639442443848]);
schmetter9Anim = schmetterling9.getControlledObject().startAnimation("idle");
schmetter9Anim.setSpeed( 1.3 );

schmetterling10 = $AM.createMeshActor("Schmetterling10", "tie_schmetterling_01.mesh" );
schmetterling10.placeIntoScene([ -13.7822866439819, -63.2723007202148, -93.3229675292969]);
schmetter10Anim = schmetterling10.getControlledObject().startAnimation("idle");
schmetter10Anim.setSpeed( 1.1 );

#Wind aus
$AM.getActor("windBoee").getControlledObject().stop()
$AM.getActor("wind").getControlledObject().stop()
$AM.getActor("windBaum").getControlledObject().stop()
$AM.getActor("windBaum2").getControlledObject().stop()

#Wölfe wegschmeißen wenn das Schnetzel aktiv ist
if (RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest51").isKnown())
	[1,2,3,4].each{|w| $AM.getActor("Wolf_"+w.to_s).setPosition( 0.0, -1000.0, 0.0 ) }
end

print("Der Wald wurde schoen");