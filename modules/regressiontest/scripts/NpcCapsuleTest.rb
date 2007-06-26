require 'testcase.rb'
require 'npc.rb'

class NpcCapsuleTest < TestCase
    def execute()
        height = 0.3
        min_base = [-2.0, 0.05, -2.0]
        max_base = [2.0, height, 2.0]
        base = $AM.createBoxPrimitiveActor("capsuleTestBase",
                                           min_base, max_base, "alpha_red")
        base.placeIntoScene(getCenter());

        npc1proxy = $GOM.createGameObject("TestPersonCapsule");
        npc1 = npc1proxy;
        npc1.setPosition(rel_pos([0.0, height, 0.0]));

        npc1.placeIntoScene();
    end
end
