require 'testcase.rb'

class GoToJobTest < TestCase
  def execute()
    followingCreature = $GOM.createGameObject("TestPerson");
	followingCreature.placeIntoScene();
	followingCreature.setPosition(getCenter());
	followingCreature.modifyAu(1000);

    job = GoToJob.new(
        followingCreature,
        $hero,
        5,
        59,
        true);
    job2 = GoToJob.new(
        followingCreature,
        getCenter(),
        0.5,
        10000000);
	$JS.addJob(job);
    $JS.addJob(job2, JobScheduler::JP_NORMAL, 60);
	
	$SCRIPT.log("GoToJobTest initialisiert")
  end
end
