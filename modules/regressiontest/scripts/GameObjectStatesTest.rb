require 'testcase.rb'

class GameObjectStatesTest < TestCase
  def initialize(center)
    super(center)

    appl2 = $GOM.createGameObject("apple", 15);
    p "appl2 created"
    appl2.placeIntoScene();
    p "set appl2 into scene"
    appl2.setPosition(rel_pos([1, 1, 1]));
    p appl2.getPosition()
  end
end