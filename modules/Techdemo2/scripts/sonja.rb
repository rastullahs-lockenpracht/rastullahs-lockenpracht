# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Sonja Bot wird geladen" );
$GOM.createGameObjectProxy("Sonja").getGameObject().placeIntoScene();
print( "Sonja Bot fertig geladen" );