# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

print( "Jarn Bot wird geladen" );

print( "Definiere Listener" );

jarn = Creature.new("Jarn", "Jarn");
jarn.setActor($AM.createMeshActor("Jarn", "men_Gennisch.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
jarn.getActor().placeIntoScene([-18.0, -6.8, 15.2]);
jarn.addAction(TalkAction.new("Jarn", "jarn.xml"))
agent = $AI.createAgent(jarn);
agent.addSteeringBehaviour(DefaultWanderBehaviour.new);
print( "Jarn Bot fertig geladen" );