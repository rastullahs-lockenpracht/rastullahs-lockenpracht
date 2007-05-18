require 'globals.rb'

class MergeableMeshTest < TestCase
    def initialize(center)
        super(center)

        $SCRIPT.log("MergeableMeshTest Start");

        test_obj1 = $GOM.createGameObject("mesh construction kit test object")
        test_obj1.setPosition(center)
        test_obj1.placeIntoScene();


        test_obj2 = $GOM.createGameObject("mesh construction kit test creature")
        test_obj2.setPosition(rel_pos([1, 0, 0]))
        test_obj2.placeIntoScene();
#        test_obj2.getActor().getControlledObject().startAnimation("walk")
        test_obj2.getInventory().addSlot("Head", "head", Item::ITEMTYPE_HELMET, false);
        test_obj2.getInventory().addSlot("Torso", "torso", Item::ITEMTYPE_ALL_ITEMS, false);
        test_item2 = $GOM.createGameObject("mesh construction kit test item 1")
        test_obj2.getInventory().hold(test_item2, "Torso")

        test_obj3 = $GOM.createGameObject("mesh construction kit test creature")
        test_obj3.setPosition(rel_pos([2, 0, 0]))
        test_obj3.placeIntoScene();
        test_obj3.getInventory().addSlot("Head", "head", Item::ITEMTYPE_HELMET, false);
        test_obj3.getInventory().addSlot("Torso", "torso", Item::ITEMTYPE_ALL_ITEMS, false);
        test_item3 = $GOM.createGameObject("mesh construction kit test item 2")
        test_obj3.getInventory().hold(test_item3, "Torso")
        test_obj3.getActor().getControlledObject().startAnimation("walk")

        test_obj4 = $GOM.createGameObject("mesh construction kit test creature")
        test_obj4.setPosition(rel_pos([3, 0, 0]))
        test_obj4.placeIntoScene();
        test_obj4.getInventory().addSlot("Head", "head", Item::ITEMTYPE_HELMET, false);
        test_obj4.getInventory().addSlot("Torso", "torso", Item::ITEMTYPE_ALL_ITEMS, false);
        test_item4 = $GOM.createGameObject("mesh construction kit test item 2")
        test_obj4.getInventory().hold(test_item4, "Torso")
        test_obj4.getInventory().dropItem("Torso")

    end
end
