require 'testcase.rb'

class GameObjectCreationTest < TestCase
  def initialize(center)
    super(center)

    appl = $GOM.createGameObject("apple");
    appl.setPosition([0,0,0]);

    appl2 = $GOM.createGameObject("apple", 15);
    appl2.setPosition([0,1,0]);
  end
end