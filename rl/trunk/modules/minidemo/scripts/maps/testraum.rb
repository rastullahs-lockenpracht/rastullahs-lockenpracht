require 'globals.rb'
require 'player.rb'
require 'torch.rb'
require 'hero.rb'

$Core.log("init map 'minidemo'...");
$World = $Core.getWorld();
$World.setSkyBox(true, "rl/dsa07");
$Core.log("skybox set");

$Core.log("Held erstellen");
hero = Hero.new;
$Core.log("Held erstellt");
$Core.log("Held in die Szene einfuegen.");
hero.getActor().placeIntoScene(0.0, 20.0, 40.0, 1.0, 0.0, 0.0, 0.0);
$Core.log("Held eingefügt.");
PlayerSettings.preparePlayer(hero);
$Core.log("Held vorbereitet.");
$Ui.setActiveCharacter(hero);
$Core.log("Held als aktiver Charakter gesetzt.");

#$Core.log("Fackel erstellen...");
#torch = Torch.new("Fackel des Grauens");
#$Core.log("Fackel erstellt.");
#torch.getActor().placeIntoScene(0.0, 20.0, 80.0, 1.0, 0.0, 0.0, 0.0);
#$Core.log("Fackel plaziert.");
$Core.log("map 'minidemo' initialisiert.");
