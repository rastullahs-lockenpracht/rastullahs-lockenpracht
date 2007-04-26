load "embed.rb"

require 'player.rb'
require 'hero.rb'
require 'doortest.rb'
require 'GameObjectCreationTest.rb'
require 'GameObjectStatesTest.rb'
require 'NpcTest.rb'
require 'NpcCapsuleTest.rb'
require 'TimeTest.rb'
require 'InventoryTest.rb'
require 'ErrorHandlingTest.rb'
require 'JobSequenceTest.rb'
require 'SoundTest.rb'
require 'effecttest.rb'
require 'LightzoneTest.rb'
require 'MapLoadingTest.rb'

# Physik aktivieren
$PM.setEnabled(true)

$SCRIPT.log("init map 'regressiontest'...")

$SCRIPT.log("Create hero")
$hero = $GOM.createGameObject("hero", 1);

$SCRIPT.log("Place hero into scene")
#$hero.setPosition([0.0, 1.0, 0.0]);
$hero.setState(GOS_IN_SCENE)



$SCRIPT.log("Prepare hero for being the active character")
PlayerSettings.preparePlayer($hero)

$SCRIPT.log("Set hero as active character")
$UI.setActiveCharacter($hero)

$SCRIPT.log("Hero ready")

$SCRIPT.log("map 'regressiontest' initialisiert.");

DoorTest.new([0.0, 0.0, -5.0])
#GameObjectCreationTest.new([5, 0, 0])
#GameObjectStatesTest.new([0, 0, 5])
NpcTest.new([-5, 0, 5])
NpcCapsuleTest.new([0, 0, 5])
TimeTest.new([-5, 0, -5])
ErrorHandlingTest.new([-10, 0, 5])
InventoryTest.new([0, 0, 10])
JobSequenceTest.new([5, 0, -5])
EffectTest.new([5, 0, 5])
SoundTest.new([10, 0, 10])
LightzoneTest.new([-10, 0, -5])
MapLoadingTest.new([-5, 0, -10])


$SCRIPT.log("tests initialisiert.");
