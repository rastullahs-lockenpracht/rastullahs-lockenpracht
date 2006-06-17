# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Jarn Bot wird geladen" );

print( "Definiere Listener" );

jarn = Creature.new("Jarn", "Jarn");
jarn.setActor($AM.createMeshActor("Jarn", "held.mesh" ));
jarn.getActor().placeIntoScene([-18.0, -6.8, 15.2]);
jarn.addAction(TalkAction.new("Jarn", "jarn.xml"))

print( "Jarn Bot fertig geladen" );