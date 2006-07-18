# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Tsafried Bot wird geladen" );

print( "Definiere Listener" );

tsafried = Creature.new("Tsafried", "Tsafried");
tsafried.setActor($AM.createMeshActor("Tsafried", "men_gennisch2.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
tsafried.getActor().placeIntoScene([11.46, -8.88, 39.43]);

bot = $DS.loadBot("Tsafried", "tsafried.xml");
bot.setNonPlayerCharacter(tsafried);
tsafried.addAction(TalkAction.new("Tsafried", "tsafried.xml"))
agent = $AI.createAgent(bot);
agent.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent.addSteeringBehaviour(DialogBehaviour.new($hero.getActor()));

print( "Tsafried Bot fertig geladen" );