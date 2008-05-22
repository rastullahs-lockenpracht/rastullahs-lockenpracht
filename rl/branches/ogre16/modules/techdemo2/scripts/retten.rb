# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

$SCRIPT.log("Rettenvariante ausgewählt");

$SCRIPT.log("Questbook aufräumen");
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest").setKnown(false)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest1").setKnown(false)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest2").setKnown(false)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest42").setKnown(false)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest42").setState(Quest::SUCCEEDED)
RulesSubsystem.getSingleton().getQuestBook().getQuest("aufenthaltsort").setKnown(false)
RulesSubsystem.getSingleton().getQuestBook().getQuest("wolfram").setKnown(false)
RulesSubsystem.getSingleton().getQuestBook().getQuest("paket").setKnown(false)
$SCRIPT.log("Questbook aufgeräumt");

RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest52").setKnown(true)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest52").setState(Quest::ASSIGNED)

$SCRIPT.log("NetterAlter wird geladen");
  $GOM.createGameObject("RettungsDruide").placeIntoScene();
$SCRIPT.log("NetterAlter geladen");


#Der Wolf wird vom Dialogscript geladen, vielleicht kann er hier auch vorbereitet werden.