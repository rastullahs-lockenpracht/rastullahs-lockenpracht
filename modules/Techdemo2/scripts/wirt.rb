# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Wirt wird geladen" );

print( "Definiere Listener" );

wirt = $AM.createMeshActor("Ein Wirt", "men_wirt_posed.mesh" );
wirt.placeIntoScene(-27.10, -6.48, -1.40, 1.0, 0.0, 0.0, 0.0);
#wirt = Creature.new("Wirt", "Ein Wirt. Er lauscht dem Bauern und lässt sich nicht stören.");
#wirt.getControlledObject().startAnimation("reden");

print( "Wirt fertig geladen" );