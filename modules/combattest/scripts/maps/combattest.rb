load 'embed.rb'

require 'globals.rb'
require 'player.rb'
require 'hero.rb'

# Physik aktivieren
$PM.setEnabled(true)

$SCRIPT.log("init map 'regressiontest'...")

$SCRIPT.log("Create hero")
$hero = $GOM.createGameObject("hero", 1)

$SCRIPT.log("Place hero into scene")
$hero.placeIntoScene()

$SCRIPT.log("Prepare hero for being the active character")
PlayerSettings.preparePlayer($hero)

$SCRIPT.log("Set hero as active character")
$UI.setActiveCharacter($hero)
$SCRIPT.log("Hero ready")

wolf = $GOM.createGameObject("Wolf")
wolf.placeIntoScene
wolf.setPosition [0.0,0.0,-20.0]
wolf.setAlignment(Creature::ALIGNMENT_ENEMY)
wolf.modifyAu(1000)

creatureController = CreatureController.new(wolf)

job = CreatureWalkPathJob.new("TestWalkJob", creatureController, Landmark.new("point", [5.0, 0.0, -10.0]))
job.addLandmark("1",[5.0,0.0,-20.0])
job.addLandmark("2",[5.0,0.0,-15.0])
$JS.addJob(job);

$SCRIPT.log("map 'combattest' initialisiert.")

