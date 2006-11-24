require 'testcase.rb'
require 'npc.rb'

class NpcTest < TestCase
  def initialize(center)
    super(center)

    npc1proxy = $GOM.createGameObjectProxy("npc");
    npc1 = npc1proxy.getGameObject();
    npc1.setProperty("meshfile", "men_alrike.mesh");
    npc1.setProperty("position", center);
    npc1.setProperty("dialogfile", "testdialog.aiml");

    npc1.placeIntoScene();
  end
end