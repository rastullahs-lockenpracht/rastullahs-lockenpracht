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

$SCRIPT.log("FieserAlter wird geladen");
$GOM.createGameObject("SchnetzelDruide").placeIntoScene();
$SCRIPT.log("FieserAlter geladen");



$SCRIPT.log("Fiese Wölfe");
load("waldwoelfe_schnetzeln.rb");
$SCRIPT.log("Fiese Wölfe geladen");


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
        steinepos = [   [ 100.0,    [21.44, -26.57, -260.17] ],
                        [ 30000.0,  [19.53, -35.26, -260.17] ],
                        [ 40000.0,  [19.92, -42.9, -261.4] ],
                        [ 60000.0,  [20.5, -30.85, -258.21]],
                        [ 20000.0,  [19.48, -40.62, -259.11] ],
                        [ 10000.0,  [19.48, -36.87, -259.11] ]
                    ]
        @steineList = Array.new
        i = 0
        steinepos.each{ |s|
            i = i+1
            stein = $AM.createMeshActor("Stein"+i.to_s, "ver_stein_turm.mesh", PhysicsManager::GT_BOX, s[0] )
		    stein.placeIntoScene( s[1] )
            stein.getPhysicalThing().setGravityOverride(true, 0.0, 0.0, 0.0)
            stein.setVisible(false)
            @steineList.push( stein )
        }
	end

	def areaLeft(anEvent)
	end

	def areaEntered(anEvent)
		$SCRIPT.log("Zone betreten, Kampfscript laden");
		load( "kampf.rb" );


		#Schrott und Schutt fliegt von oben herab
		$AM.getActor("Steinschlag").getControlledObject().play()
		
        @steineList.each{ |s| 
            s.getPhysicalThing().setGravityOverride(false)
            s.getPhysicalThing().unfreeze()
            s.setVisible(true)
        }

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
$GameEveMgr.addSphereAreaListener( turmEingang, 1.000, areaListener, RlScript::QUERYFLAG_PLAYER );