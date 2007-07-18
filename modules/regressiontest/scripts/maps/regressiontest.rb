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
require 'MaterialSlotTest.rb'
require 'MergeableMeshTest.rb'
require 'WalkJobTest.rb'
require 'SelectorTest.rb'


# Physik aktivieren
$PM.setEnabled(true)

$SCRIPT.log("init map 'regressiontest'...")

$SCRIPT.log("Prepare hero for being the active character")
#$hero = $GOM.getGameObject(1); # single mesh hero
$hero = $GOM.getGameObject(10); # construction kit hero
PlayerSettings.preparePlayer($hero)

$SCRIPT.log("Set hero as active character")
$UI.setActiveCharacter($hero)

$SCRIPT.log("Hero ready")

$SCRIPT.log("begin tests.");

 MaterialSlotTest.new([-15, 0, -5]).run()
    LightzoneTest.new([-10, 0, -5]).run()
ErrorHandlingTest.new([-10, 0, 5]).run()
         TimeTest.new([ -5, 0, -5]).run()
   MapLoadingTest.new([ -5, 0, -10]).run()
          NpcTest.new([ -5, 0, 5]).run()
         DoorTest.new([  0, 0, -5]).run()
	  WalkJobTest.new([  0, 0, -3]).run()
   NpcCapsuleTest.new([  0, 0, 5]).run()
    InventoryTest.new([  0, 0, 10]).run()
  JobSequenceTest.new([  5, 0, -5]).run()
     SelectorTest.new([  5, 0, 0]).run()
       EffectTest.new([  5, 0, 5]).run()
MergeableMeshTest.new([ 10, 0, 0]).run()
        SoundTest.new([ 10, 0, 10]).run()

$SCRIPT.log("tests initialisiert.");

$SCRIPT.log("map 'regressiontest' initialisiert.");
