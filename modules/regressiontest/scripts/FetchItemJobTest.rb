require "testcase.rb"

class FetchItemJobTest < TestCase
    def setUp()
        @fetchingCreature = $GOM.createGameObject("TestPerson");
        @fetchingCreature.placeIntoScene();
        @fetchingCreature.setPosition(getCenter());
        @fetchingCreature.modifyAu(1000);
        
        @fetchItem1 = $GOM.createGameObject("Kurzschwert");
        @fetchItem1.setPosition(rel_pos([-15, 0, 2]));
        @fetchItem1.placeIntoScene();
        
        @fetchItem2 = $GOM.createGameObject("Kurzschwert");
        chest = $GOM.createGameObject("chest");
        chest.addItem(@fetchItem2);
        chest.setPosition(rel_pos([2, 0, 15]));
        chest.placeIntoScene();
    end
    
    def execute()
        setUp();
        
        queue = JobQueue.new();
        queue.add(FetchItemJob.new(@fetchingCreature, @fetchItem1, "Left Hand", 9999999999));
        queue.add(FetchItemJob.new(@fetchingCreature, @fetchItem2, "Right Hand", 9999999999));
        $JS.addJob(queue);
    end
end
