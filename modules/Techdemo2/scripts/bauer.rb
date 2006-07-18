# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Bauer wird geladen" );

bowerActor = $AM.createMeshActor("Bauer", "men_Gennisch.mesh" );
 
bower = Creature.new("Ein Bauer", "Ein Bauer. Er ist im Gespräch vertieft und lässt sich nicht stören.");
bower.setActor( bowerActor );
 
bowerActor.placeIntoScene(-27.10, -6.5, -0.32, 1.0, 0.0, 0.0, 0.0);
bowerActor.getControlledObject().startAnimation("reden");

print( "Bauer fertig geladen" );