require 'testcase.rb'
require 'npc.rb'

class InventoryTest < TestCase
  def initialize(center)
    super(center)

    longsword = $GOM.createGameObjectProxy("longsword");

    hero = $GOM.getGameObjectProxy(1).getGameObject();
    hero.getInventory().hold(longsword.getGameObject(), "Right Hand");

  end
end