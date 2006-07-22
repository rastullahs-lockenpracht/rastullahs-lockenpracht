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
druide = Creature.new("Alter Mann", "Alter Mann");
druide.setActor($AM.createMeshActor("Alter Mann", "men_druide.mesh", PhysicsManager::GT_BOX, 900.0));
#druide.getActor().placeIntoScene([-5.80, -38.23, -19.97]);#Testposition
druide.getActor().placeIntoScene([20.72, -47.08, -260.28]); 

bot = $DS.loadBot("Alter Mann", "druide52.xml");
bot.setNonPlayerCharacter(druide);
druide.addAction(TalkAction.new("Alter Mann", "druide52.xml"))
#agent = $AI.createAgent(bot);
#agent.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
#agent.addSteeringBehaviour(DialogBehaviour.new($hero.getActor()));
$SCRIPT.log("NetterAlter geladen");


#Der Wolf wird vom Dialogscript geladen, vielleicht kann er hier auch vorbereitet werden.