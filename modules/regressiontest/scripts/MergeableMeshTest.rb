require 'globals.rb'

class MergeableMeshTest < TestCase
    def execute()
        $SCRIPT.log(">MergeableMeshTest Start");

        $SCRIPT.log(">>MergeableMeshTest Actor Start");
        testMergeableActor();
        $SCRIPT.log(">>MergeableMeshTest Actor End");

        $SCRIPT.log(">>MergeableMeshTest Slots Start");
        testMergeableSlots();
        $SCRIPT.log(">>MergeableMeshTest Slots End");

        $SCRIPT.log(">>MergeableMeshTest Inventory Start");
        testMergeableInventory();
        $SCRIPT.log(">>MergeableMeshTest Inventory End");

        $SCRIPT.log(">MergeableMeshTest End");
    end

    def testMergeableSlots()
        test_obj1 = $GOM.createGameObject("mesh construction kit test object")
        test_obj1.setPosition(rel_pos([1, 0, 0]))
        test_obj1.placeIntoScene();

        test_obj2 = $GOM.createGameObject("mesh construction kit test creature")
        test_obj2.setPosition(rel_pos([2, 0, 0]))
        test_obj2.placeIntoScene();
#        test_obj2.getActor().getControlledObject().startAnimation("Walk")
        test_obj2.getInventory().addSlot("Head", "head", 0, Item::ITEMTYPE_HELMET, RlScript::SLOT_SUBMESH);
        test_obj2.getInventory().addSlot("Right Hand", "Bip01 R SlotHand", 0, Item::ITEMTYPE_ALL_ITEMS, RlScript::SLOT_BONE);
        test_obj2.getInventory().addSlot("Armor", "armor", 0, Item::ITEMTYPE_ALL_ITEMS, RlScript::SLOT_SUBMESH);
        test_item2 = $GOM.createGameObject("mesh construction kit test item 1")
        test_obj2.getInventory().hold(test_item2, "Armor")

        test_obj3 = $GOM.createGameObject("mesh construction kit test creature")
        test_obj3.setPosition(rel_pos([3, 0, 0]))
        test_obj3.placeIntoScene();
        test_obj3.getInventory().addSlot("Head", "head", 0, Item::ITEMTYPE_HELMET, RlScript::SLOT_SUBMESH);
        test_obj3.getInventory().addSlot("Right Hand", "Bip01 R SlotHand", 0, Item::ITEMTYPE_ALL_ITEMS, RlScript::SLOT_BONE);
        test_obj3.getInventory().addSlot("Armor", "armor", 0, Item::ITEMTYPE_ALL_ITEMS, RlScript::SLOT_SUBMESH);
        test_item3 = $GOM.createGameObject("mesh construction kit test item 2")
        test_obj3.getInventory().hold(test_item3, "Armor")
        test_obj3.getActor().getControlledObject().startAnimation("Walk")
        test_obj3.getActor().getPhysicalThing().updateCollisionHull()

        test_obj4 = $GOM.createGameObject("mesh construction kit test creature")
        test_obj4.setPosition(rel_pos([4, 0, 0]))
        test_obj4.placeIntoScene();
        test_obj4.getInventory().addSlot("Head", "head", 0, Item::ITEMTYPE_HELMET, RlScript::SLOT_SUBMESH);
        test_obj4.getInventory().addSlot("Right Hand", "Bip01 R SlotHand", 0, Item::ITEMTYPE_ALL_ITEMS, RlScript::SLOT_BONE);
        test_obj4.getInventory().addSlot("Armor", "armor", 0, Item::ITEMTYPE_ALL_ITEMS, RlScript::SLOT_SUBMESH);
        test_item4 = $GOM.createGameObject("mesh construction kit test item 2")
        test_obj4.getInventory().hold(test_item4, "Armor")
        test_obj4.getInventory().dropItem("Armor")

        $SCRIPT.log("Give swords to all creatures.");
        sword2 = $GOM.createGameObject("Kurzschwert");
        test_obj2.getInventory().hold(sword2, "Right Hand");
        sword3 = $GOM.createGameObject("Kurzschwert");
        test_obj3.getInventory().hold(sword3, "Right Hand");
        sword4 = $GOM.createGameObject("Kurzschwert");
        test_obj4.getInventory().hold(sword4, "Right Hand");
    end

    def testMergeableActor()
      baseActor1 = $AM.createMeshActor("", "men_human_female_torso.mesh", GT_ELLIPSOID, 75, true)
      baseActor1.getControlledObject().setBaseMeshPart("torso")
      actorToAdd11 = $AM.createMeshActor("", "men_human_female_head_02.mesh")
      actorToAdd12 = $AM.createMeshActor("", "men_human_female_legs_pants_01_long.mesh")
      actorToAdd13 = $AM.createMeshActor("", "men_human_female_armor_kroetenhaut.mesh")
      actorToAdd14 = $AM.createMeshActor("", "men_human_female_feet_boots_01.mesh")
      baseActor1.merge(actorToAdd11, "head")
      baseActor1.merge(actorToAdd12, "legs")
      baseActor1.merge(actorToAdd13, "armor")
      baseActor1.merge(actorToAdd14, "feet")
      baseActor1.placeIntoScene(rel_pos([1, 0, 2]))

      baseActor2 = $AM.createMeshActor("", "men_human_female_torso.mesh", GT_ELLIPSOID, 75, true)
      baseActor2.getControlledObject().setBaseMeshPart("torso")
      actorToAdd2 = $AM.createMeshActor("", "men_human_female_head_02.mesh")
      actorToAdd2.placeIntoScene(rel_pos([2.5, 0, 2]))
      baseActor2.merge(actorToAdd2, "head")
      baseActor2.placeIntoScene(rel_pos([2, 0, 2]))

      baseActor3 = $AM.createMeshActor("", "men_human_female_torso.mesh", GT_ELLIPSOID, 75, true)
      baseActor3.getControlledObject().setBaseMeshPart("torso")
      baseActor3.placeIntoScene(rel_pos([3, 0, 2]))
      actorToAdd3 = $AM.createMeshActor("", "men_human_female_head_02.mesh")
      baseActor3.merge(actorToAdd3, "head")

      baseActor4 = $AM.createMeshActor("", "men_human_female_torso.mesh", GT_ELLIPSOID, 75, true)
      baseActor4.getControlledObject().setBaseMeshPart("torso")
      baseActor4.placeIntoScene(rel_pos([4, 0, 2]))
      actorToAdd4 = $AM.createMeshActor("", "men_human_female_head_02.mesh")
      actorToAdd2.placeIntoScene(rel_pos([4.5, 0, 2]))
      baseActor4.merge(actorToAdd4, "head")
    end
    
    def testMergeableInventory()
      testObj = $GOM.createGameObject("kroetenhaut")
      $hero.getInventory().hold(testObj, "Armor")
    end
end
