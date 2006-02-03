# Nur eingebettet starten
load( "embed.rb" );

print( "GameEvent-Tests wird geladen" );

print( "Definiere Listener" );
# Definition des GameAreaListeners
class DunkleEckenLachListener < GameAreaListener

	def areaLeft(anEvent)
		print( "Raus - " +  anEvent.getProvokingActor().getName() );
		$GameEveMgr.removeAreaListener(self)
		print( "Raus - lösch " );
	end
	def areaEntered(anEvent)
		print( "Rein - " + anEvent.getProvokingActor().getName() );
		anEvent.getSource().getActor().getControlledObject().play();
		$UI.showMessageWindow( "Das ist die dunkle Ecke" );
	end
end

held = $AM.getActor( "Held" );
held.setQueryMask( Actor::QGF_PLAYER );

$SCRIPT.log("Kugel-Zentrum Actor erstellen");
# kugelDings = $AM.createEmptyActor( "Kugel-Zentrum" );
kugelDings = $AM.createSoundSampleActor( "Kugel-Zentrum", "lachen.ogg" );
$SCRIPT.log("Kugel-Zentrum Actor in die Szene einfügen");
kugelDings.placeIntoScene( [415.0, 5.0, -300.0] );

$SCRIPT.log("DunkleEckenLachListener erstellen");
areaListener = DunkleEckenLachListener.new();

$SCRIPT.log("DunkleEckenLachListener hinzufügen");
$GameEveMgr.addSphereAreaListener( kugelDings, 50.0, areaListener, Actor::QGF_PLAYER );



print( "GameEvent-Tests fertig geladen" );