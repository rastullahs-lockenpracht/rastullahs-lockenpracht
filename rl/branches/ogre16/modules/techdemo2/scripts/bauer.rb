# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Bauer wird geladen" );

bower = $GOM.createGameObject("Bauer");
bower.placeIntoScene()

bowerAnim = bower.getActor().getControlledObject().startAnimation("reden");
bowerAnim.setSpeed( 0.9 );

print( "Bauer fertig geladen" );