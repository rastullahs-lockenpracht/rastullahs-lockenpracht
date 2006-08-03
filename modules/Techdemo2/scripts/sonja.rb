# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Sonja Bot wird geladen" );

print( "Definiere Listener" );

sonja = Creature.new("Sonja", "Sonja");
sonja.setActor($AM.createMeshActor("Sonja", "men_sonja.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
sonja.getActor().placeIntoScene([-32.40, -6.46, 4.03],[0.0, 120.94, 0.0]);

bot = $DS.loadBot("Sonja", "sonja.xml");
bot.setNonPlayerCharacter(sonja);
sonja.addAction(TalkAction.new("Sonja", "sonja.xml"))
agent = $AI.createAgent(bot);
agent.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));

print( "Sonja Bot fertig geladen" );