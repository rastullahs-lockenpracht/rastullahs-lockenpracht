class AStarTest3
    def initialize(center)
  		wpgraph = $WPGM.createWayPointGraph();
  		wpgraph.load("astargrid.wpg.xml")
  
      lmp = LandmarkPath.new("TotalTollerTest");
      lmp.addPoint("P6", [0, 0, 8]);
      lmp.addPoint("P5", [-8, 0, 8]);
      lmp.addPoint("P4", [0, 0, 0]);
      lmp.addPoint("P3", [8, 0, 8]);
      lmp.addPoint("P2", [8, 0, -2]);
      lmp.addPoint("P1", [0, 0, -2]);
      
      15.times do |x|
        creature = $GOM.createGameObject("TestWalkPerson")
        creature.setPosition([-x,0,0])
        creature.placeIntoScene();
        creature.modifyAu(1000);
        agent = AgentManager::getSingleton().createAgent(creature);
        behaviour = WalkPathBehaviour.new();
        behaviour.setLandmarkPath(lmp);
        agent.getCurrentState().addSteeringBehaviour(behaviour);
      end
      # the following code uses the AStar instead of a LandmarkPath
  		#walkingCreature = $GOM.createGameObject("TestWalkPerson")
      #walkingCreature.setPosition([0,0,0])
      #walkingCreature.placeIntoScene
      #walkingCreature.modifyAu(1000)
      
      #landmark = Landmark.new("point", [5.0, 0.0, 0.0]);
      #agent = AgentManager::getSingleton().createAgent(walkingCreature);
      #behaviour = WalkPathBehaviour.new();
      #agent.getCurrentState().addSteeringBehaviour(behaviour);
      #behaviour.calcPathToLandmark(landmark, wpgraph);      
    end
end

