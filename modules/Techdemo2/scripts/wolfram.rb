# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Wolfram Bot wird geladen" );

print( "Definiere Listener" );

wolfram = Creature.new("Wolfram", "Wolfram");
wolfram.setActor($AM.createMeshActor("Wolfram", "held.mesh" ));
wolfram.getActor().placeIntoScene([-20.5, -6.7, 3.2]);
wolfram.addAction(TalkAction.new("Wolfram", "wolfram.xml"))

print( "Wolfram Bot fertig geladen" );