# Nur eingebettet starten
load( "embed.rb" );

print( "DialogDemo wird geladen" );

print( "Definiere Listener" );
# Definition des GameAreaListeners
class DialogListener < GameAreaListener
	def initialize(source)	
		super();
		# Load a bot from a bot definition xml file
		@dialogChar = $DS.loadBot( "Alrike", "startup_test.xml" );
		@dialogChar.setNonPlayerCharacter( source.getGameObject() );
	end
	
	def areaEntered(anEvent)
		# Set the game objects for the dialogcharacter and its dialog partner
		@dialogChar.setPlayerCharacter( anEvent.getProvokingActor().getGameObject() );
		print( anEvent.getProvokingActor().getName() + " moechte mit " + anEvent.getSource().getActor().getName() + " reden" );
		$WF.showDialog( @dialogChar );
	end

	def areaLeft(anEvent)
		print( "Dialogpartner ist außerhalb der Reichweite" );
	end
end

$SCRIPT.log("Binde DialogListener an KreislaufHeld");
dialogActor = $AM.getActor( "DialogHeld" );
$GameEveMgr.addSphereAreaListener( dialogActor, 1.0, DialogListener.new(dialogActor), Actor::QGF_PLAYER );

print( "DialogDemo fertig geladen" );