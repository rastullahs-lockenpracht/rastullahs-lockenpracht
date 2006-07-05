# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );
 
print( "Jarn Bot wird geladen" );
 
print( "Definiere Listener" );
 
jarn = Creature.new("Jarn", "Jarn");
#jarn.setActor($AM.createMeshActor("Jarn", "men_Gennisch.mesh"));
jarn.setActor($AM.createMeshActor("Jarn", "men_Gennisch.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
jarn.getActor().placeIntoScene([-18.0, -6.8, 15.2]);

bot=$DS.loadBot("Jarn", "jarn.xml");
bot.setNonPlayerCharacter(jarn);
jarn.addAction(TalkAction.new("Jarn", "jarn.xml"));
agent = $AI.createAgent(bot);
agent.addSteeringBehaviour(DefaultWanderBehaviour.new);
agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent.addSteeringBehaviour(DialogBehaviour.new($hero.getActor()));
print("Jarn Bot fertig geladen");
