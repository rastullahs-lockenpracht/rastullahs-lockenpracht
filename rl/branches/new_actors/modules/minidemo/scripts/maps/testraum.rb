require 'globals.rb'
require 'player.rb'
require 'torch.rb'
require 'hero.rb'

$Core.log("init map 'minidemo'...");
$World = $Core.getWorld();
$World.setSkyBox(true, "rl/dsa07");
$Core.log("skybox set");

hero = Hero.new;
hero.getActor().placeIntoScene(0.0, 20.0, 40.0, 1.0, 0.0, 0.0, 0.0);
PlayerSettings.preparePlayer(hero);
$Ui.setActiveCharacter(hero);

torch = Torch.new("Fackel des Grauens");
torch.getActor().placeIntoScene(position, orientierung);
