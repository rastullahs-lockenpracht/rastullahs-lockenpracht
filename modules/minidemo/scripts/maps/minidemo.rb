require 'globals.rb'
require 'player.rb'
require 'torch.rb'
require 'hero.rb'
require 'door.rb'
require 'switch.rb'

$CORE.log("init map 'minidemo'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "rl/dsa07")
$CORE.log("skybox set");

$CORE.log("Held erstellen");
hero = Hero.new;
$CORE.log("Held erstellt");
$CORE.log("Held in die Szene einfuegen.");
hero.getActor().placeIntoScene(0.0, 20.0, 40.0, 1.0, 0.0, 0.0, 0.0);
$CORE.log("Held eingefügt.");
PlayerSettings.preparePlayer(hero);
$CORE.log("Held vorbereitet.");
$UI.setActiveCharacter(hero);
$CORE.log("Held als aktiver Charakter gesetzt.");

# $CORE.log("Fackel erstellen...");
# torch = Torch.new("Fackel des Grauens");
# $CORE.log("Fackel erstellt.");
# torch.getActor().placeIntoScene(0.0, 20.0, 80.0, 1.0, 0.0, 0.0, 0.0);
# $CORE.log("Fackel plaziert.");

$CORE.log("Türen reinsetzen")
door1 = Door.new("Tuer_1", false, true);
door1.getActor().placeIntoScene(1168.0, -75.0, -110.0, 1.0, 0.0, 0.0, 0.0);
door1.getActor().yaw(-90.0)

gtuer = Door.new("Tuer_2", false, false);
gtuer.getActor().placeIntoScene(1168.0, -75.0, 88.0, 1.0, 0.0, 0.0, 0.0);
gtuer.getActor().yaw(-90.0)
$CORE.log("Türen fertig")


$CORE.log("Hebel reinsetzen")
hebel = Switch.new( "Hebel" );
hebel.getActor().placeIntoScene( 160.0, 24.0, 160.0, 1.0, 0.0, 0.0, 0.0 );
hebel.getActor().yaw(-90.0);
p hebel
$CORE.log("Hebel fertig");


$CORE.log("map 'minidemo' initialisiert.");