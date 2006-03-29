# Nur eingebettet starten
load( "embed.rb" );

print( "DialogDemo wird geladen" );

print( "Definiere Listener" );
# Definition des GameAreaListeners
class DialogListener < GameAreaListener
	def initialize(source)	
		super();
		# Load a bot from a bot definition xml file
		@dialogChar = $DS.loadBot( "Alrik.xml" );
		@dialogChar.setDialogPartner( source.getGameObject() );
	end
	
	def areaEntered(anEvent)
		# Set the game objects for the dialogcharacter and its dialog partner
		@dialogChar.setDialogCharacter( anEvent.getProvokingActor().getGameObject() );
		print( anEvent.getProvokingActor().getName() + " moechte mit " + anEvent.getSource().getActor().getName() + " reden" );
		$WM.showDialog( @dialogChar );
	end

	def areaLeft(anEvent)
		print( "Dialogpartner ist au�erhalb der Reichweite" );
	end
end

$SCRIPT.log("Binde DialogListener an KreislaufHeld");
kreislauf = $AM.getActor( "KreisLaufHeld" );
$GameEveMgr.addSphereAreaListener( kreislauf, 2.0, DialogListener.new(kreislauf), Actor::QGF_PLAYER );

print( "DialogDemo fertig geladen" );