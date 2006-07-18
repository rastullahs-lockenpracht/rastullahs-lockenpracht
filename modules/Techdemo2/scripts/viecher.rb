

# Nur eingebettet starten
load( "embed.rb" );
 
print( "Alle Viecher werden geladen" );

schmetterling = $AM.createMeshActor("Schmetterling", "tie_schmetterling_01.mesh" );
schmetterling.placeIntoScene(13.54, -7.96, 38.05, 1.0, 0.0, 0.0, 0.0);

#held.getControlledObject().startAnimation("idle");
schmetterlingAnim = schmetterling.getControlledObject().startAnimation( "Idle" );
#schmetterlingAnim.setSpeed( 1.00 );

print("Alle Viecher fertig geladen");
