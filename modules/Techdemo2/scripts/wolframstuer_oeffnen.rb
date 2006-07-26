# Nur eingebettet starten
load( "embed.rb" );
require( "npc.rb" );
require( "door.rb" );

tuer = $AM.getActor("WolframsTuer")
tuer.getControlledObject().startAnimation("auf",1.0,1)
tuer.getControlledObject().replaceAnimation("zu", "auf", 1.0, 1)
tuer.getPhysicalThing().fitToPose("zu")

wolfram = $AM.getActor("Wolfram")
wolfram.getControlledObject().startAnimation("gehen",1.0,3)
