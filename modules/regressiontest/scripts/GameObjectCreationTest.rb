require 'testcase.rb'

class GameObjectCreationTest < TestCase
  def execure()
    appl = $GOM.createGameObject("apple");
    appl.setPosition(rel_pos[0,0,0]);

    appl2 = $GOM.createGameObject("apple", 15);
    appl2.setPosition(rel_pos[0,1,0]);
  end
end