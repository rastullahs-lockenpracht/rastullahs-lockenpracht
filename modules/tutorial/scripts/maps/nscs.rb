load "embed.rb"
load( "npc.rb" );

#==== NSC für 01.aiml ====

nsc01 = Creature.new("AIML01", "AIML01");
nsc01.setActor($AM.createMeshActor("AIML01", "men_gennisch2.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
nsc01.getActor().placeIntoScene([ -1.46, 0.00, -3.57],[0.0, 180.0, 0.0]);

bot01 = $DS.loadBot("AIML01", "nscs.xml");
bot01.setNonPlayerCharacter(nsc01);
nsc01.addAction(TalkAction.new("AIML01", "nscs.xml"));
agent01 = $AI.createAgent(bot01);
agent01.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent01.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent01.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));


#==== NSC für 02.aiml ====

nsc02 = Creature.new("AIML02", "AIML02");
nsc02.setActor($AM.createMeshActor("AIML02", "men_gennisch2.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
nsc02.getActor().placeIntoScene([ -0.46, 0.00, -3.57],[0.0, 180.0, 0.0]);

bot02 = $DS.loadBot("AIML02", "nscs.xml");
bot02.setNonPlayerCharacter(nsc02);
nsc02.addAction(TalkAction.new("AIML02", "nscs.xml"));
agent02 = $AI.createAgent(bot02);
agent02.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent02.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent02.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));


#==== NSC für 03.aiml ====

nsc03 = Creature.new("AIML03", "AIML03");
nsc03.setActor($AM.createMeshActor("AIML03", "men_gennisch2.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
nsc03.getActor().placeIntoScene([ 0.7, 0.00, -3.57],[0.0, 180.0, 0.0]);

bot03 = $DS.loadBot("AIML03", "nscs.xml");
bot03.setNonPlayerCharacter(nsc03);
nsc03.addAction(TalkAction.new("AIML03", "nscs.xml"));
agent03 = $AI.createAgent(bot03);
agent03.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent03.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent03.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));


#==== NSC für 04.aiml ====

nsc04 = Creature.new("AIML04", "AIML04");
nsc04.setActor($AM.createMeshActor("AIML04", "men_gennisch2.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
nsc04.getActor().placeIntoScene([ 1.82, 0.00, -3.57],[0.0, 180.0, 0.0]);

bot04 = $DS.loadBot("AIML04", "nscs.xml");
bot04.setNonPlayerCharacter(nsc04);
nsc04.addAction(TalkAction.new("AIML04", "nscs.xml"));
agent04 = $AI.createAgent(bot04);
agent04.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent04.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent04.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));


#==== NSC für 05.aiml ====

nsc05 = Creature.new("AIML05", "AIML05");
nsc05.setActor($AM.createMeshActor("AIML05", "men_gennisch2.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
nsc05.getActor().placeIntoScene([ 2.88, -0.01, -0.58],[0.0, 90.0, 0.0]);

bot05 = $DS.loadBot("AIML05", "nscs.xml");
bot05.setNonPlayerCharacter(nsc05);
nsc05.addAction(TalkAction.new("AIML05", "nscs.xml"));
agent05 = $AI.createAgent(bot05);
agent05.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent05.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent05.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));



#==== NSC für 06.aiml ====

nsc06 = Creature.new("AIML06", "AIML06");
nsc06.setActor($AM.createMeshActor("AIML06", "men_gennisch2.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
nsc06.getActor().placeIntoScene([ 2.88, -0.01, 1.0],[0.0, 90.0, 0.0]);

bot06 = $DS.loadBot("AIML06", "nscs.xml");
bot06.setNonPlayerCharacter(nsc06);
nsc06.addAction(TalkAction.new("AIML06", "nscs.xml"));
agent06 = $AI.createAgent(bot06);
agent06.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent06.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent06.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));




#==== NSC für 08.aiml ====

nsc08 = Creature.new("AIML08", "AIML08");
nsc08.setActor($AM.createMeshActor("AIML08", "men_gennisch2.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
nsc08.getActor().placeIntoScene([ 2.88, -0.01, 2.36],[0.0, 90.0, 0.0]);

bot08 = $DS.loadBot("AIML08", "nscs.xml");
bot08.setNonPlayerCharacter(nsc08);
nsc08.addAction(TalkAction.new("AIML08", "nscs.xml"));
agent08 = $AI.createAgent(bot08);
agent08.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent08.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent08.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));


#==== NSC für 09.aiml ====

nsc09 = Creature.new("AIML09", "AIML09");
nsc09.setActor($AM.createMeshActor("AIML09", "men_gennisch2.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
nsc09.getActor().placeIntoScene([ 1.97, -0.01, 3.78],[0.0, 0.0, 0.0]);

bot09 = $DS.loadBot("AIML09", "nscs.xml");
bot09.setNonPlayerCharacter(nsc09);
nsc09.addAction(TalkAction.new("AIML09", "nscs.xml"));
agent09 = $AI.createAgent(bot09);
agent09.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent09.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent09.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));



#==== NSC für 10.aiml ====

nsc10 = Creature.new("AIML10", "AIML10");
nsc10.setActor($AM.createMeshActor("AIML10", "men_gennisch2.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
nsc10.getActor().placeIntoScene([ 0.13, -0.01, 3.78],[0.0, 0.0, 0.0]);

bot10 = $DS.loadBot("AIML10", "nscs.xml");
bot10.setNonPlayerCharacter(nsc10);
nsc10.addAction(TalkAction.new("AIML10", "nscs.xml"));
agent10 = $AI.createAgent(bot10);
agent10.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent10.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent10.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));

