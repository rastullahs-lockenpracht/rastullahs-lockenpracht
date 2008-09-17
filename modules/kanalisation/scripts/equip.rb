require 'npc.rb'
require 'torch.rb'
require 'clothing.rb'

hero = PartyManager.getSingleton().getActiveCharacter();

itemToAdd = $GOM.createGameObject("men_human_female_legs_pants_01_long");
hero.getInventory().hold(itemToAdd, "Trousers");

itemToAdd = $GOM.createGameObject("men_human_female_feet_boots_01");
hero.getInventory().hold(itemToAdd, "Boots");

itemToAdd = $GOM.createGameObject("men_human_female_cloth_short");
hero.getInventory().hold(itemToAdd, "Clothes");

itemToAdd = $GOM.createGameObject("torch");
hero.getInventory().hold(itemToAdd, "Right Hand");
