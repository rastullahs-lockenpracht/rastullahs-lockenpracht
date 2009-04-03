require 'testcase.rb'
require 'npc.rb'

class NpcTest < TestCase
    def execute()        
        height = 0.3
        min_base = [-3.0, 0.05, -3.0]
        max_base = [3.0, height, 3.0]
        base = $AM.createBoxPrimitiveActor("DialogTestBase",
                                           min_base, max_base, "alpha_blue")
        base.placeIntoScene(getCenter());

        npc1 = $GOM.createGameObject("TestPerson");
        npc1.setPosition(rel_pos([0.0, height, 0.0]));
        npc1.placeIntoScene();
        npc1.getActor().getPhysicalThing().setUpConstraint();


        npc2 = $GOM.createGameObject("TestPerson2");
        npc2.setPosition(rel_pos([2.2, height, 2.2]));
        npc2.setProperty("dialog", "test_3pers_dialog")
        npc2.placeIntoScene();
        npc2.getActor().getPhysicalThing().setUpConstraint();

        npc3 = $GOM.createGameObject("TestPerson");
        npc3.setPosition(rel_pos([1.8, height, 1.8]));
        npc3.setProperty("dialog", "test_3pers_dialog")
        npc3.setName("TestPerson_byName");
        npc3.placeIntoScene();
        npc3.getActor().getPhysicalThing().setUpConstraint();
    end
end
