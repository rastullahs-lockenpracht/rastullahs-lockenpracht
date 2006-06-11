# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "DialogDemo wird geladen" );

print( "Definiere Listener" );

dialogHeld = Creature.new("DialogHeld", "DialogHeld");
dialogHeld.setActor($AM.createMeshActor("DialogHeld", "held.mesh" ));
dialogHeld.getActor().placeIntoScene([-1.7, 0.0, -5.3]);
dialogHeld.addAction(TalkAction.new("Alrike", "startup_test.xml"))

print( "DialogDemo fertig geladen" );