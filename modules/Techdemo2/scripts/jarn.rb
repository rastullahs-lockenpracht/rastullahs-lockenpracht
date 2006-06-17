# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Jarn Bot wird geladen" );

print( "Definiere Listener" );

dialogHeld = Creature.new("Jarn", "Jarn");
dialogHeld.setActor($AM.createMeshActor("Jarn", "held.mesh" ));
dialogHeld.getActor().placeIntoScene([-18.0, -6.8, 15.2]);
dialogHeld.addAction(TalkAction.new("Jarn", "jarn.xml"))

print( "Jarn Bot fertig geladen" );