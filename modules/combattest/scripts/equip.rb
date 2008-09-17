require 'npc.rb'
require 'clothing.rb'
	
	$SCRIPT.log("aktiven hero geten");
	hero = PartyManager.getSingleton().getActiveCharacter();

    
    itemToAdd = $GOM.createGameObject("men_human_female_legs_pants_01_long");
    hero.getInventory().hold(itemToAdd, "Trousers");
    itemToAdd = $GOM.createGameObject("men_human_female_cloth_short");
    hero.getInventory().hold(itemToAdd, "Clothes");
    itemToAdd = $GOM.createGameObject("men_human_female_feet_boots_01");
    hero.getInventory().hold(itemToAdd, "Boots");