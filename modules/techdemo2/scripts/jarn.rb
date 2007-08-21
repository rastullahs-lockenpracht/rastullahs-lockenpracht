# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );
 
print( "Jarn Bot wird geladen" );
 
print( "Definiere Listener" );

 
#jarn = Creature.new("Jarn", "Jarn");
#jarn.setActor($AM.createMeshActor("Jarn", "men_gennisch2.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
#jarn.getActor().placeIntoScene([ -36.16, -6.25, 18.12],[0.0, -23.40, 0.0]);

#bot = $AI.loadBot("Jarn", "jarn.xml");
#bot.setNonPlayerCharacter(jarn);
#jarn.addAction(TalkAction.new("Jarn", "jarn.xml"));
#agent = $AGM.createAgent(bot);
#agent.addSteeringBehaviour(DefaultIdleBehaviour.new);
##agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
#agent.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));

$GOM.createGameObject("Jarn").placeIntoScene();

print("Jarn Bot fertig geladen");
