# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Tsafried Bot wird geladen" );

print( "Definiere Listener" );

tsafried = Creature.new("Tsafried", "Tsafried");
tsafried.setActor($AM.createMeshActor("Tsafried", "held.mesh" ));
tsafried.getActor().placeIntoScene([-30.4, -6.5, 5.7]);
tsafried.addAction(TalkAction.new("Tsafried", "tsafried.xml"))

print( "Tsafried Bot fertig geladen" );