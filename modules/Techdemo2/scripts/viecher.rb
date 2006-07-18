# Nur eingebettet starten
load( "embed.rb" );
 
print( "Alle Viecher werden geladen" );

schmetterling1 = $AM.createMeshActor("Schmetterling1", "tie_schmetterling_01.mesh" );
schmetterling1.placeIntoScene(13.54, -5.96, 38.05, 1.0, 0.0, 0.0, 0.0);
schmetterling1.getControlledObject().startAnimation("idle");

schmetterling2 = $AM.createMeshActor("Schmetterling2", "tie_schmetterling_02.mesh" );
schmetterling2.placeIntoScene(11.82, -6.50, 43.04, 1.0, 0.0, 0.0, 0.0);
schmetter2Anim = schmetterling2.getControlledObject().startAnimation("fliegen");
schmetter2Anim.setSpeed( 1.25 );

hase = $AM.createMeshActor("Hase", "tie_hase_braun.mesh" );
hase.placeIntoScene(14.9, -9.1, 42.5, 1.0, 0.0, 0.0, 0.0);
  hase.getControlledObject().startAnimation("idle"); 

print("Alle Viecher fertig geladen");
