load "embed.rb"

require 'player.rb'
require 'hero.rb'

$SCRIPT.log("Prepare hero for being the active character")
hero = $GOM.getGameObject(10);
hero.setPosition([ -35.15, 2.69, 26.09]);
hero.setOrientation([ [ -4.43614335381426e-005, -0.999999940395355, -3.53998184436932e-005 ] , 67.3891525268555]);
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

load "equip.rb"
