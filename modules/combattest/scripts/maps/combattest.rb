require 'player.rb'
require 'hero.rb'

# Physik aktivieren
$PM.setEnabled(true)

$SCRIPT.log("init map 'regressiontest'...")

$SCRIPT.log("Create hero")
$hero = $GOM.createGameObjectProxy("hero", 1);

$SCRIPT.log("Place hero into scene")
$hero.getGameObject().placeIntoScene()

$SCRIPT.log("Prepare hero for being the active character")
PlayerSettings.preparePlayer($hero.getGameObject())

$SCRIPT.log("Set hero as active character")
$UI.setActiveCharacter($hero.getGameObject())
$SCRIPT.log("Hero ready")

wolf = $GOM.createGameObjectProxy("Wolf").getGameObject()
wolf.placeIntoScene
wolf.setPosition [0.0,0.0,-5.0]
wolf.setAlignment(Creature::ALIGNMENT_ENEMY)

$SCRIPT.log("map 'combattest' initialisiert.")

