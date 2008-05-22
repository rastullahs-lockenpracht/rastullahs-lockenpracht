# Nur eingebettet starten
load( "embed.rb" );
 
print( "Alle Viecher werden geladen" );

#Schmetterlinge in Tsafrieds Naturecke
schmetterling1 = $AM.createMeshActor("Schmetterling1", "tie_schmetterling_01.mesh" );
schmetterling1.placeIntoScene(13.54, -5.96, 38.05, 1.0, 0.0, 0.0, 0.0);
schmetterling1.getControlledObject().startAnimation("idle");

schmetterling2 = $AM.createMeshActor("Schmetterling2", "tie_schmetterling_02.mesh" );
schmetterling2.placeIntoScene(11.82, -6.50, 43.04, 1.0, 0.0, 0.0, 0.0);
schmetter2Anim = schmetterling2.getControlledObject().startAnimation("idle");
schmetter2Anim.setSpeed( 1.25 );

hase = $AM.createMeshActor("Fussballhase", "tie_hase_braun.mesh", PhysicsManager::GT_BOX, 3.0 );
hase.placeIntoScene(14.9, -9.1, 42.5, 1.0, 0.0, 0.0, 0.0);
#hase.getControlledObject().startAnimation("idle"); 

#hase = $AM.createMeshActor("Fussballhase", "tie_wolf_01.mesh");
#hase.placeIntoScene(14.9, -9.1, 42.5, 1.0, 0.0, 0.0, 0.0);
#hase.getControlledObject().startAnimation("ausweichen"); 

#Bauern
$SCRIPT.log("Unterhaltung1 laden");
unterhaltung1 = $AM.createSoundSampleActor( "unterhaltung1", "bauern_001.ogg" ); 
unterhaltung1.getControlledObject().set3d(true);
$SCRIPT.log(" Loopen");
unterhaltung1.getControlledObject().setLooping( true );
$SCRIPT.log(" Setzen");
unterhaltung1.placeIntoScene([-27.26, -6.49, -0.80]);
$SCRIPT.log(" Abspielen");
unterhaltung1.getControlledObject().set3d(true);
unterhaltung1.getControlledObject().play();
$SCRIPT.log(" Volumen");
unterhaltung1.getControlledObject().setVolume( 0.7 )

$SCRIPT.log("Unterhaltung1 fertig");

print("Alle Viecher fertig geladen");



