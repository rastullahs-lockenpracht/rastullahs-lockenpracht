# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

$SCRIPT.log("Schnetzelvariante ausgewählt");

$SCRIPT.log("Questbook aufräumen");
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest").setKnown(false)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest1").setKnown(false)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest2").setKnown(false)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest41").setKnown(false)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest41").setState(Quest::SUCCEEDED)
RulesSubsystem.getSingleton().getQuestBook().getQuest("aufenthaltsort").setKnown(false)
RulesSubsystem.getSingleton().getQuestBook().getQuest("wolfram").setKnown(false)
RulesSubsystem.getSingleton().getQuestBook().getQuest("paket").setKnown(false)
$SCRIPT.log("Questbook aufgeräumt");

RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest51").setKnown(true)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest51").setState(Quest::ASSIGNED)

#Der alte Mann aus dem Wald I  -  Tagebuch aktivieren 
	questBook = RulesSubsystem.getSingleton().getQuestBook()
	questBook.addJournalEntry("Der alte Mann aus dem Wald I",
	"Der verlassene Turm, von dem mir erzählt wurde, ist ganz in der Nähe. Ich kann ihn über den Baumwipfeln schon sehen. Hier sollte ich wieder vorsichtiger sein. Wer weiß auf welche Weise mit der alte Kauz auflauern wird. Sobald ich ihn sehe werde ich ihn fangen und festsetzen. Das wichtigste ist jetzt die Sicherheit der Dorfbewohner, und die ist nur gewährleistet wenn der Alte Mann unschädlich gemacht worden ist.");

$SCRIPT.log("FieserAlter wird geladen");
	druide = Creature.new("Alter Mann", "Alter Mann");
	druide.setActor($AM.createMeshActor("Alter Mann", "men_druide.mesh", PhysicsManager::GT_BOX, 900.0));
	druide.getActor().placeIntoScene([21.78, -35.28, -264.45]);
	#druide.getActor().placeIntoScene([-5.80, -38.23, -19.97]);

	bot = $DS.loadBot("Alter Mann", "druide51.xml");
	bot.setNonPlayerCharacter(druide);
	druide.addAction(TalkAction.new("Alter Mann", "druide51.xml"))
	#agent = $AI.createAgent(bot);
	#agent.addSteeringBehaviour(DefaultIdleBehaviour.new);
	#agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
	#agent.addSteeringBehaviour(DialogBehaviour.new($hero.getActor()));
$SCRIPT.log("FieserAlter geladen");



$SCRIPT.log("HarmlosesTier wird geladen");
	#Irgendwas läuft in der Map rum, damits schöner aussieht.
$SCRIPT.log("HarmlosesTier geladen");


#Zone am Eingang um kampf.rb auszulösen und es spannend zu machen#
$SCRIPT.log("Turmeingang: Eingangszone Actor erstellen");
turmEingang = $AM.createEmptyActor( "Eingangszone" );
$SCRIPT.log("Turmeingang: Eingangszone Actor in die Szene einfügen");
turmEingang.placeIntoScene( [14.71, -47.06, -262.59] );


print( "Definiere TurmeingangListener" );
# Definition des GameAreaListeners
class TurmeingangListener < GameAreaListener
	def initialize()
		super()
	end

	def areaLeft(anEvent)
	end

	def areaEntered(anEvent)
		$SCRIPT.log("Zone betreten, Kampfscript laden");
		load( "kampf.rb" );


		#Schrott und Schutt fliegt von oben herab
		$AM.getActor("Steinschlag").getControlledObject().play()

		stein1 = $AM.createMeshActor("Stein1", "ver_stein_turm.mesh", PhysicsManager::GT_BOX, 100.0 );
		stein1.placeIntoScene([21.44, -26.57, -260.17]);

		stein2 = $AM.createMeshActor("Stein2", "ver_stein_turm.mesh", PhysicsManager::GT_BOX, 30000.0 );
		stein2.placeIntoScene([19.53, -35.26, -260.17]);
		
		stein3 = $AM.createMeshActor("Stein3", "ver_stein_turm.mesh", PhysicsManager::GT_BOX, 40000.0 );
		stein3.placeIntoScene([19.92, -42.9, -261.4]);

		stein4 = $AM.createMeshActor("Stein4", "ver_stein_turm.mesh", PhysicsManager::GT_BOX, 60000.0 );
		stein4.placeIntoScene([20.5, -30.85, -258.21]);

		stein5 = $AM.createMeshActor("Stein5", "ver_stein_turm.mesh", PhysicsManager::GT_BOX, 20000.0 );
		stein5.placeIntoScene([19.48, -40.62, -259.11]);

		stein6 = $AM.createMeshActor("Stein6", "ver_stein_turm.mesh", PhysicsManager::GT_BOX, 10000.0 );
		stein6.placeIntoScene([19.48, -36.87, -259.11]);

		
		#Staubpartikeleffekt an Koordinaten

		#Geräusch
		@mFiesesLachenSound = $AM.createSoundSampleActor("FiesesLachen","altermann004.ogg");
		@mFiesesLachenSound.getControlledObject().set3d(false);
		@mFiesesLachenSound.getControlledObject().load();
		@mFiesesLachenSound.getControlledObject().play();


		
		$GameEveMgr.removeAreaListener(self)
	
	end
end

$SCRIPT.log("Turmeingang: TurmeingangListener erstellen");
areaListener = TurmeingangListener.new();

$SCRIPT.log("Turmeingang: TurmeingangListener hinzufügen");
$GameEveMgr.addSphereAreaListener( turmEingang, 1.000, areaListener, Actor::QGF_PLAYER );