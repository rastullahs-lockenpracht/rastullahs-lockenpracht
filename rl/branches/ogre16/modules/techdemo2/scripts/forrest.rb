# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );
 
print( "Lauf, Forrest, Lauf!" );
 
print( "Definiere Listener" );
 
#forrest = Creature.new("Forrest", "Forrest");
#forrest.setActor($AM.createMeshActor("Forrest", "men_Gennisch.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
#forrest.getActor().placeIntoScene([-20.83, -6.70, 19.46]);
#
#bot=$AI.loadBot("Forrest", "forrest.xml");
#bot.setNonPlayerCharacter(forrest);
#forrest.addAction(TalkAction.new("Forrest", "forrest.xml"));
#agent = $AGM.createAgent(bot);
#agent.addSteeringBehaviour(DefaultWanderBehaviour.new);
#agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
#agent.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));
$GOM.createGameObject("Forrest").placeIntoScene();
print("Forrest fertig geladen");
