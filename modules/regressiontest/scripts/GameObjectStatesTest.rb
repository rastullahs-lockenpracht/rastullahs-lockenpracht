require 'testcase.rb'

class GameObjectStatesTest < TestCase
  def initialize(center)
    super(center)

    appl2 = $GOM.createGameObjectProxy("apple", 15);
    p "appl2 created"
    appl2.getGameObject().placeIntoScene();
    p "set appl2 into scene"
    appl2.getGameObject().setPosition(rel_pos([1, 1, 1]));
    p appl2.getGameObject().getPosition()
  end
end