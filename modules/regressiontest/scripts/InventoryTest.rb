require 'testcase.rb'
require 'npc.rb'

class InventoryTest < TestCase
  def execute()
    $SCRIPT.log("InventoryTest started.");
    hero = PartyManager.getSingleton().getActiveCharacter();
    backpack = $GOM.createGameObject("leather backpack");
    #backpack.addAction(ShowContainerContentAction.new());
    #backpack.setPosition(center);
    #backpack.placeIntoScene();
    
    p "InventoryTest #1: Put an object a creature's slot."
    hero.getInventory().hold(backpack, "Back");
    
    itemToAdd = $GOM.createGameObject("men_human_female_legs_pants_01_long");
    hero.getInventory().getItem("Back").addItem(itemToAdd);
    itemToAdd = $GOM.createGameObject("men_human_female_cloth_short");
    hero.getInventory().getItem("Back").addItem(itemToAdd);
    itemToAdd = $GOM.createGameObject("men_human_female_feet_boots_01");
    hero.getInventory().getItem("Back").addItem(itemToAdd);
    
    $SCRIPT.log("InventoryTest #2: Put a loaded object into container.");
    sword_loaded_to_inventory = $GOM.createGameObject("Kurzschwert");
    hero.getInventory().getItem("Back").addItem(sword_loaded_to_inventory);

    $SCRIPT.log("InventoryTest #3: Put an object from the scene into container.");
    sword_in_scene_to_inventory = $GOM.createGameObject("Kurzschwert");
    sword_in_scene_to_inventory.placeIntoScene();
    hero.getInventory().getItem("Back").addItem(sword_in_scene_to_inventory);

    $SCRIPT.log("InventoryTest #4: Put an object attached to the hero into container.");
    sword_held_to_inventory = $GOM.createGameObject("Kurzschwert");
    hero.getInventory().hold(sword_held_to_inventory, "Left Hand");
    #hero.getInventory().dropItem("Left Hand")
    hero.getInventory().getItem("Back").addItem(sword_held_to_inventory);

    $SCRIPT.log("InventoryTest #5: Put an object from a container into the hero's hand.");
    sword_inventory_to_hold = $GOM.createGameObject("Kurzschwert");
    hero.getInventory().getItem("Back").addItem(sword_inventory_to_hold)
    #hero.getInventory().getItem("Back").removeItem(sword_inventory_to_hold)
    hero.getInventory().hold(sword_inventory_to_hold, "Right Hand");
    
	p "Start torch"
	backpack.addItem($GOM.createGameObject("torch"))

    p "InventoryTest Expectation: Hero should stand with a backpack on her back and a sword attached to the right hand, there should be no sword on the floor or in the left hand";
  end
end
