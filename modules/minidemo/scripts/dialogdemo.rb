# Nur eingebettet starten
load( "embed.rb" );

print( "DialogDemo wird geladen" );

print( "Definiere Listener" );
# Definition des GameAreaListeners
class DialogListener < GameAreaListener
	def areaEntered(anEvent)
		# Load a bot from a bot definition xml file
		$dialogChar = $DS.loadBot( "Alrik.xml" );
		# Set the game objects for the dialogcharacter and its dialog partner
		$dialogChar.setDialogCharacter( anEvent.getSource().getActor.getGameObject() );
		$dialogChar.setDialogPartner( anEvent.getProvokingActor().getGameObject() );
		print( anEvent.getProvokingActor().getName() + " moechte mit " + anEvent.getSource().getActor().getName() + " reden" );
		$WM.showDialog( $dialogChar );
	end
	def areaLeft(anEvent)
		print( "Dialogpartner ist außerhalb der Reichweite" );
	end
end

$SCRIPT.log("Binde DialogListener an KreislaufHeld");
$GameEveMgr.addSphereAreaListener( $AM.getActor( "KreisLaufHeld" ), 0.5, DialogListener.new, Actor::QGF_PLAYER );

print( "DialogDemo fertig geladen" );