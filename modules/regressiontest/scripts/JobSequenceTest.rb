require 'jobs/jobsequence.rb'
require 'jobs/soundjobs.rb'
require 'jobs/animationjobs.rb'

class JobSequenceTestAction < Action
  def initialize
    super("checktime", "Zeit ansagen lassen.");
  end
  
  def canDo(go, user)
    true
  end
  
  def doAction(go, user, target)
    jobSequence(
            [
                _PlayAnimation(go.getActor(), "auf"),
                _PlaySound3d("doorcreak.ogg", go.getPosition()),
                2,
                _PlaySound3d("lachen.ogg", go.getPosition()),
                2,
                _PlayAnimation(go.getActor(), "zu"),
                _PlaySound3d("doorcreak.ogg", go.getPosition())
            ]
    );
  end
end

class JobSequenceTest < TestCase
    def initialize(center)
        super(center)

        # Define a base box to place everything else on.
        height = 0.3
        min_base = [-2.0, 0.05, -2.0]
        max_base = [2.0, height, 2.0]
        base = $AM.createBoxPrimitiveActor("JobSequenceTestBase",
                                           min_base, max_base, "alpha_green")
        base.placeIntoScene(center);

        chest = $GOM.createGameObject(
            "JobSequenceTestChest");
        chest.addAction(JobSequenceTestAction.new());
        chest.placeIntoScene();
        chest.setPosition(rel_pos([0.0, height, 0.0]));

        $SCRIPT.log("JobSequenceTest initialisiert");
    end
end

