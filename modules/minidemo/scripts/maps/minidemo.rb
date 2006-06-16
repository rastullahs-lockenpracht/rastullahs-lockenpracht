load "embed.rb"

require 'player.rb'
require 'torch.rb'
require 'hero.rb'
require 'door.rb'
require 'switch.rb'
require 'truhe.rb'
require 'door_opener.rb'
require 'heiltrank.rb'
require 'crate.rb'

# Physik aktivieren
$PM.setEnabled(true);

$SCRIPT.log("init map 'minidemo'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "rl/dsa07", 100.0)
$SCRIPT.log("skybox set");

$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(-0.8, -2.0, 3.0);
sunlight.getControlledObject().setCastShadows(false);
sunlight.getControlledObject().setDiffuseColour(0.8,0.8,0.7);
$SCRIPT.log("Tageslicht erstellt.");

$SCRIPT.log("Held erstellen");
$hero = Hero.new;
p $hero.getName()
$SCRIPT.log("Held erstellt");
$SCRIPT.log("Held in die Szene einfuegen.");
$hero.getActor().placeIntoScene([0.0, 0.2, 0.4]);
$SCRIPT.log("Held eingefügt.");
PlayerSettings.preparePlayer($hero);
$SCRIPT.log("Held vorbereitet.");
$UI.setActiveCharacter($hero);
$SCRIPT.log("Held als aktiver Charakter gesetzt.");

$SCRIPT.log("Kiste einsetzen");
crate = Crate.new("Eine Kiste", "Eine Kiste.\nWas mag da drin sein?");
crate.getActor().placeIntoScene([1.80, 1.80, 2.20]);

$SCRIPT.log("Türen einsetzen")
door1 = Door.new("Eine Tür links", "Eine Holztür", [13.68, -0.75, -1.70], [0, 90, 0]);
door1.addActions(false, true)

gtuer = Door.new("Eine Tür rechts", "Eine Holztür.\nDiese Tür hat weder Klinke noch Schlösser", [13.68, -0.75, 0.60], [0, 90, 0]);
gtuer.addActions(false, false)
$SCRIPT.log("Türen fertig")


$SCRIPT.log("Hebel reinsetzen")
hebel = Switch.new( "Hebel" );
hebel.getActor().placeIntoScene([13.30, 0.25, 1.80]);
hebel.getActor().pitch(90.0)
hebel.getActor().roll(90.0)
$SCRIPT.log("Hebel fertig");

doorprocessor = SecretDoorOpener.new(gtuer, hebel)

$SCRIPT.log("Truhe reinsetzen")
chest = Chest.new( "Truhe", false );
chest.getActor().placeIntoScene([17.50, -3.50, -1.05]);
chest.getActor().yaw(-90.0);
$SCRIPT.log("Truhe fertig");

$SCRIPT.log("Traenke reinsetzen")
trank = Heiltrank.new( "Trank1", "Eine Flasche mit einer geheimnisvollen Fluessigkeit", "obj_heiltrank01.mesh", "A" );
chest.addItem(trank);
trank = Heiltrank.new( "Trank2", "Eine andere Flasche mit einer anderen geheimnisvollen Fluessigkeit", "obj_heiltrank01.mesh", "B" );
chest.addItem(trank);
$SCRIPT.log("Traenke fertig");

$World.setFog( World::FOG_EXP, [0.8,0.8,1.0,0.5], 0.00009, 0.2, 1.0);



load "kreislauf.rb"
load "timer.rb"
load "area.rb"
load "dialogdemo.rb"

$SCRIPT.log("map 'minidemo' initialisiert.");
