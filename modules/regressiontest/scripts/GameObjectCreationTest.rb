require 'testcase.rb'

class GameObjectCreationTest < TestCase
  def initialize(center)
    super(center)

    appl = $GOM.createGameObjectProxy("apple");
    appl.getGameObject().setPosition([0,0,0]);

    appl2 = $GOM.createGameObjectProxy("apple", 15);
    appl2.getGameObject().setPosition([0,1,0]);
  end
end