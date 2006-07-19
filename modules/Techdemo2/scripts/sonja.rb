# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Sonja Bot wird geladen" );

print( "Definiere Listener" );

sonja = Creature.new("Sonja", "Sonja");
sonja.setActor($AM.createMeshActor("Sonja", "men_sonja.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
sonja.getActor().placeIntoScene([-32.2882194519043, -6.46541738510132, 4.17184495925903],[0.0, 115.0, 0.0]);

bot = $DS.loadBot("Sonja", "sonja.xml");
bot.setNonPlayerCharacter(sonja);
sonja.addAction(TalkAction.new("Sonja", "sonja.xml"))
agent = $AI.createAgent(bot);
agent.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent.addSteeringBehaviour(DialogBehaviour.new($hero.getActor()));

print( "Sonja Bot fertig geladen" );