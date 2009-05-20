require 'util/vector_util.rb'
require 'testcase.rb'
require 'globals.rb'

class DirectorErrorAction < Action
  def initialize
    super("DirectorErrorAction", "Raises an Exception.");
  end
  
  def canDo(go, user, target)
    true
  end
  
  def doAction(go, user, target)
    raise "This is an Error. Go handle it!";
  end
end

class ErrorHandlingTest < TestCase
    def execute()
        # Define a base box to place everything else on.
        height = 0.3
        min_base = [-2.0, 0.05, -2.0]
        max_base = [2.0, height, 2.0]
        base = $AM.createBoxPrimitiveActor("ErrorHandling", min_base, max_base,
                                          "alpha_orange")
        base.placeIntoScene(getCenter());

        eh = $GOM.createGameObject("ErrorHandling");
        eh.addAction(DirectorErrorAction.new());
        eh.placeIntoScene();
        eh.setPosition(rel_pos([0.0, height, 0.0]));

        $SCRIPT.log("TimeTest initialisiert");
    end
end
