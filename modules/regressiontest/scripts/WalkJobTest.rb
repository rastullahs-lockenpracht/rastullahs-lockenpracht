require 'testcase.rb'

class WalkJobTest < TestCase
  def execute()
    walkingCreature = $GOM.createGameObject("TestPerson")
	walkingCreature.placeIntoScene
	walkingCreature.setPosition(getCenter())
	walkingCreature.modifyAu(1000)

    movingCreature = MovingCreature.new(walkingCreature)

    job = CreatureWalkPathJob.new("WalkJobTest", movingCreature, Landmark.new("point", [5.0, 0.0, -10.0]))
    job.addLandmark("1",[5.0,0.0,-20.0])
    job.addLandmark("2",[5.0,0.0,-15.0])
	$JS.addJob(job);
  end
end