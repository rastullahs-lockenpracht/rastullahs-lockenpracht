load 'embed.rb'

require 'player.rb'
require 'hero.rb'

# Physik aktivieren
$PM.setEnabled(true)

$SCRIPT.log("init map 'regressiontest'...")

$SCRIPT.log("Create hero")
$hero = $GOM.createGameObject("hero", 1);

$SCRIPT.log("Place hero into scene")
$hero.placeIntoScene()

$SCRIPT.log("Prepare hero for being the active character")
PlayerSettings.preparePlayer($hero)

$SCRIPT.log("Set hero as active character")
$UI.setActiveCharacter($hero)
$SCRIPT.log("Hero ready")

wolf = $GOM.createGameObject("Wolf")
wolf.placeIntoScene
wolf.setPosition [0.0,0.0,-5.0]
wolf.setAlignment(Creature::ALIGNMENT_ENEMY)

wolf2 = $GOM.createGameObject("Wolf2")
wolf2.placeIntoScene
wolf2.setPosition [0.0,0.0,-3.0]
wolf2.setAlignment(Creature::ALIGNMENT_ENEMY)

_movingCreature = MovingCreature.new(wolf2)

_job = CreatureWalkPathJob.new("TestWalkJob", _movingCreature, Landmark.new("point", [2.0, 0.0, -10.0]))

$SCRIPT.log("map 'combattest' initialisiert.")

