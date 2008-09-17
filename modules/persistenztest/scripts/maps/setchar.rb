load "embed.rb"

require 'player.rb'
require 'hero.rb'
require 'MapChange.rb'

$SCRIPT.log("Prepare hero for being the active character")
hero = $GOM.getGameObject(10);
PlayerSettings.preparePlayer(hero)

$SCRIPT.log("Set hero as active character")
PartyManager.getSingleton().setActiveCharacter(hero)

$SCRIPT.log("Hero ready")
itemToAdd = $GOM.createGameObject("men_human_female_legs_pants_01_long");
hero.getInventory().hold(itemToAdd, "Trousers");
itemToAdd = $GOM.createGameObject("men_human_female_cloth_short");
hero.getInventory().hold(itemToAdd, "Clothes");
itemToAdd = $GOM.createGameObject("men_human_female_feet_boots_01");
hero.getInventory().hold(itemToAdd, "Boots");
