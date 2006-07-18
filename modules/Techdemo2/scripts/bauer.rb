# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Bauer wird geladen" );

print( "Definiere Listener" );

bower = $AM.createMeshActor("Ein Bauer", "men_Gennisch.mesh" );
bower.placeIntoScene(-27.10, -6.5, -0.32, 1.0, 0.0, 0.0, 0.0);
#bower = Creature.new("Ein Bauer", "Ein Bauer. Er ist im Gespräch vertieft und lässt sich nicht stören.");
bower.getControlledObject().startAnimation("reden");

print( "Bauer fertig geladen" );