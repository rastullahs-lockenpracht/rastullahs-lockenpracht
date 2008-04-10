require 'npc.rb'
require 'torch.rb'

hero = $UI.getActiveCharacter();

$SCRIPT.log("backpack erstellen");
#backpack = $GOM.createGameObject("leather backpack");
#backpack.addAction(ShowContainerContentAction.new());
#backpack.setPosition(center);
#backpack.placeIntoScene();

#$SCRIPT.log("inventar fuellen!!!");
#hero.getInventory().hold(backpack, "Back");

itemToAdd = $GOM.createGameObject("men_human_female_leg_pants_01_long");
hero.getInventory().hold(itemToAdd, "Trousers");
#itemToAdd = $GOM.createGameObject("men_human_female_cloth_short");
#hero.getInventory().hold(itemToAdd, "Clothes");
itemToAdd = $GOM.createGameObject("men_human_female_feet_boots_01");
hero.getInventory().hold(itemToAdd, "Boots");

itemToAdd = $GOM.createGameObject("torch");
hero.getInventory().hold(itemToAdd, "Right Hand");
