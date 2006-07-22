# Nur eingebettet starten
load( "embed.rb" );

#hier wird Sonja2 geladen, wenn in 51 der Druide und in 52 der Wolf tot ist
#zudem Musikwechsel, Questwechsel, alles was toll ist für das Ende.

load( "npc.rb" );

print( "Sonja version Map2 wird geladen" );
print( "Definiere Listener" );
sonja = Creature.new("Sonja", "Sonja");
sonja.setActor($AM.createMeshActor("Sonja", "men_sonja.mesh", PhysicsManager::GT_BOX, 0.0));
sonja.getActor().placeIntoScene([2.70, -37.79, -23.48]);

bot = $DS.loadBot("Sonja", "sonja2.xml");
bot.setNonPlayerCharacter(sonja);
sonja.addAction(TalkAction.new("Sonja", "sonja2.xml"))
agent = $AI.createAgent(bot);
#agent.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
#agent.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));

print( "Sonja version Map2 fertig geladen" );

#Musik
$AM.getActor("kampfMusik").getControlledObject().stop()
$AM.getActor("ruchinsRuhe").getControlledObject().play()

RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest51").setState(Quest::SUCCEEDED)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest52").setState(Quest::SUCCEEDED)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest6").setState(Quest::ASSIGNED)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest6").setKnown(true)

#Die Welt wird grün
load("wald.rb");
