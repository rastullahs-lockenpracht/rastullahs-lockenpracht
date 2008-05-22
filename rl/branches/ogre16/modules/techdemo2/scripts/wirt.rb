# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Wirt wird geladen" );

print( "Definiere Listener" );

wirt = $GOM.createGameObject("Wirt");
wirt.placeIntoScene()
 
wirtAnim = wirt.getActor().getControlledObject().startAnimation("reden");
wirtAnim.setSpeed( 1.05 );
wirt.getActor().getControlledObject().startAnimation("idle");

print( "Wirt fertig geladen" );