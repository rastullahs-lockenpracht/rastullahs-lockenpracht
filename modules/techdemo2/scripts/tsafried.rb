# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Tsafried Bot wird geladen" );

print( "Definiere Listener" );

$GOM.createGameObjectProxy("Tsafried").getGameObject().placeIntoScene();

print( "Tsafried Bot fertig geladen" );