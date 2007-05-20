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
require 'MergeableMeshTest.rb'

# Physik aktivieren
$PM.setEnabled(true)

$SCRIPT.log("init map 'regressiontest'...")

$SCRIPT.log("Prepare hero for being the active character")
$hero = $GOM.getGameObject(1);
PlayerSettings.preparePlayer($hero)

$SCRIPT.log("Set hero as active character")
$UI.setActiveCharacter($hero)

$SCRIPT.log("Hero ready")

$SCRIPT.log("begin tests.");

         DoorTest.new([0, 0, -5])
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
MergeableMeshTest.new([10, 0, 0])


$SCRIPT.log("tests initialisiert.");

$SCRIPT.log("map 'regressiontest' initialisiert.");
