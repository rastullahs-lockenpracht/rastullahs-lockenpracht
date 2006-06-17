# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Sonja Bot wird geladen" );

print( "Definiere Listener" );

sonja = Creature.new("Sonja", "Sonja");
sonja.setActor($AM.createMeshActor("Sonja", "held.mesh" ));
sonja.getActor().placeIntoScene([-26.7, -6.6, 22.9]);
sonja.addAction(TalkAction.new("Sonja", "sonja.xml"))

print( "Sonja Bot fertig geladen" );