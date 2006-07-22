# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Wirt wird geladen" );

print( "Definiere Listener" );

wirtActor = $AM.createMeshActor("Wirt", "men_wirt.mesh" );
 
wirt = Creature.new("Wirt", "Ein Wirt. Er lauscht dem Bauern und lässt sich nicht stören.");
wirt.setActor( wirtActor );
 
wirtActor.placeIntoScene([-27.10, -6.48, -1.40], [0, 180, 0]);
wirtActor.getControlledObject().startAnimation("reden");

print( "Wirt fertig geladen" );