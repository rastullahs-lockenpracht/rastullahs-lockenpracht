require 'util/vector_util.rb'
require 'testcase.rb'
require 'globals.rb'

class TimeCheckAction < Action
  def initialize
    super("checktime", "Zeit ansagen lassen.");
  end
  
  def canDo(go, user, target)
    true
  end
  
  def doAction(go, user, target)
      # Get time
      timestring = $DM.getCurrentDate().toString();
      # and show message box with it.
      $WF.showMessageWindow("Es ist jetzt " + timestring);
  end
end

class TimeTest < TestCase
    def execute()
        # Define a base box to place everything else on.
        height = 0.3
        min_base = [-2.0, 0.05, -2.0]
        max_base = [2.0, height, 2.0]
        base = $AM.createBoxPrimitiveActor("TimeTestBase", min_base, max_base,
                                          "alpha_orange")
        base.placeIntoScene(getCenter());

        scheit = $GOM.createGameObject("Holzscheit");
        scheit.addAction(TimeCheckAction.new());
        scheit.placeIntoScene();
        scheit.setPosition(rel_pos([0.0, height, 0.0]));

        $SCRIPT.log("TimeTest initialisiert");
    end
end


