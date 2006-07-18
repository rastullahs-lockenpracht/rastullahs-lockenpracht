# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );
 
print( "Jarn Bot wird geladen" );
 
print( "Definiere Listener" );
 
jarn = Creature.new("Jarn", "Jarn");
#jarn.setActor($AM.createMeshActor("Jarn", "men_Gennisch2.mesh"));
jarn.setActor($AM.createMeshActor("Jarn", "men_Gennisch2.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
jarn.getActor().placeIntoScene([-8.04, -6.93, 14.66]);
#jarn.getControlledObject().startAnimation("idle");

bot=$DS.loadBot("Jarn", "jarn.xml");
bot.setNonPlayerCharacter(jarn);
jarn.addAction(TalkAction.new("Jarn", "jarn.xml"));
agent = $AI.createAgent(bot);
agent.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent.addSteeringBehaviour(DialogBehaviour.new($hero.getActor()));
print("Jarn Bot fertig geladen");
