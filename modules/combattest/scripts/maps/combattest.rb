load 'embed.rb'

require 'globals.rb'
require 'player.rb'
require 'hero.rb'
require 'mckhero.rb'

# Physik aktivieren
$PM.setEnabled(true)

$SCRIPT.log("init map 'regressiontest'...")

$SCRIPT.log("Create hero")
$hero = $GOM.createGameObject("mckhero", 1)

$SCRIPT.log("Place hero into scene")
$hero.placeIntoScene()

sword = $GOM.createGameObject("Kurzschwert");
$hero.getInventory().ready(sword, "Right Hand");

$SCRIPT.log("Prepare hero for being the active character")
PlayerSettings.preparePlayer($hero)

$SCRIPT.log("Set hero as active character")
PartyManager.getSingleton().setActiveCharacter($hero)
$SCRIPT.log("Hero ready")

wolf = $GOM.createGameObject("Wolf")
wolf.placeIntoScene
wolf.setPosition [0.0,0.0,-11.0]
wolf.setAlignment(Creature::ALIGNMENT_ENEMY)
wolf.modifyAu(1000)

schnauze = $GOM.createGameObject("Wolfsschnauze");
wolf.getInventory().ready(schnauze, "Snout");

#job = CreatureWalkPathJob.new("TestWalkJob", wolf, Landmark.new("point", [5.0, 0.0, -10.0]))
#job.addLandmark("1",[5.0,0.0,-20.0])
#job.addLandmark("2",[5.0,0.0,-15.0])
#$JS.addJob(job);

load "equip.rb"

$SCRIPT.log("map 'combattest' initialisiert.")

