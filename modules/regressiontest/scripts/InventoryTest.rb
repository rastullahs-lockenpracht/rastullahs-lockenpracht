require 'testcase.rb'
require 'npc.rb'

class InventoryTest < TestCase
  def initialize(center)
    super(center)

    longsword = $GOM.createGameObjectProxy("shortsword").getGameObject();

    $SCRIPT.log("InventoryTest #1: Put an object a creature's slot.");
    hero = $GOM.getGameObjectProxy(1).getGameObject();
    hero.getInventory().hold(longsword, "Right Hand");

    $SCRIPT.log("InventoryTest #2: Put an object a creature's slot.");
    backpack = $GOM.createGameObjectProxy("leather backpack").getGameObject();
    backpack.setVolume(5, 5);
    hero.getInventory().hold(backpack, "Back");

    $SCRIPT.log("InventoryTest #3: Put a loaded object into container.");
    sword_loaded_to_inventory = $GOM.createGameObjectProxy("shortsword").getGameObject();
    hero.getInventory().getItem("Back").addItem(sword_loaded_to_inventory);

    $SCRIPT.log("InventoryTest #4: Put an object from the scene into container.");
    sword_in_scene_to_inventory = $GOM.createGameObjectProxy("shortsword").getGameObject();
    sword_in_scene_to_inventory.placeIntoScene();
    hero.getInventory().getItem("Back").addItem(sword_in_scene_to_inventory);

    $SCRIPT.log("InventoryTest #5: Put an object attached to the hero into container.");
    sword_held_to_inventory = $GOM.createGameObjectProxy("shortsword").getGameObject();
    hero.getInventory().hold(sword_held_to_inventory, "Left Hand");
    hero.getInventory().getItem("Back").addItem(sword_held_to_inventory);

    p "InventoryTest Expectation: Hero should stand with a backpack on her back and a sword attached to the right hand, there should be no sword on the floor or in the left hand";
  end
end