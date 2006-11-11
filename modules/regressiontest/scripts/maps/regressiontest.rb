load "embed.rb"

require 'player.rb'
require 'hero.rb'
require 'doortest.rb'
require 'GameObjectCreationTest.rb'
require 'GameObjectStatesTest.rb'
require 'NpcTest.rb'

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

$SCRIPT.log("map 'regressiontest' initialisiert.");

DoorTest.new([0.0, 0.0, -5.0])
#GameObjectCreationTest.new([5, 0, 0])
#GameObjectStatesTest.new([0, 0, 5])
NpcTest.new([10, 0, 0])

$SCRIPT.log("tests initialisiert.");