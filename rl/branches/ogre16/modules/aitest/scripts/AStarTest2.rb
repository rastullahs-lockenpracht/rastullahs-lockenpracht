class AStarTest2
    def initialize(center)
		wpgraph = $WPGM.createWayPointGraph();
		wpgraph.load("astargrid.wpg.xml")

		walkingCreature = $GOM.createGameObject("TestWalkPerson")
    walkingCreature.setPosition([0,0,0])
    walkingCreature.placeIntoScene
    walkingCreature.modifyAu(1000)
    
    job = CreatureWalkPathJob.new(
      "WalkJobAStarTest", 
      walkingCreature, 
      Landmark.new("point", [5.0, 0.0, 0.0]))
    job.addLandmark("1",[-5.0,0.0,-5.0])
    job.addLandmark("2",[10.0,0.0,-10.0])
    job.setWayPoints(wpgraph)
    $JS.addJob(job);

    end
end

