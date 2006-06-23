# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "DialogHeld Bot wird geladen" );

print( "Definiere Listener" );

DialogHeld = Creature.new("DialogHeld", "Sonja");
DialogHeld.setActor($AM.createMeshActor("DialogHeld", "held.mesh" ));
DialogHeld.getActor().placeIntoScene([-1.7, 0.0, -5.3]);
DialogHeld.addAction(TalkAction.new("DialogHeld", "dialogheld.xml"))

print( "DialogHeld Bot fertig geladen" );