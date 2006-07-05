# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Sonja Bot wird geladen" );

print( "Definiere Listener" );

sonja = Creature.new("Sonja", "Sonja");
sonja.setActor($AM.createMeshActor("Sonja", "men_Gennisch.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
sonja.getActor().placeIntoScene([-26.7, -6.6, 22.9]);

bot = $DS.loadBot("Sonja", "sonja.xml");
bot.setNonPlayerCharacter(sonja);
sonja.addAction(TalkAction.new("Sonja", "sonja.xml"))
agent = $AI.createAgent(bot);
agent.addSteeringBehaviour(DefaultWanderBehaviour.new);
agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent.addSteeringBehaviour(DialogBehaviour.new($hero.getActor()));

print( "Sonja Bot fertig geladen" );