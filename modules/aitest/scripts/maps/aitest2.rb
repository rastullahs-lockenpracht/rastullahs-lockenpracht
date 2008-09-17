load "embed.rb"

require 'player.rb'
require 'hero.rb'
require 'debugkeys.rb'

#require 'doortest.rb'

require 'WallSet1.rb'
require 'WallSet2.rb'

require 'NpcTest.rb'
require 'NpcCapsuleTest.rb'

require 'WayPointTest.rb'
#require 'TimeTest.rb'
#require 'InventoryTest.rb'
#require 'ErrorHandlingTest.rb'
#require 'JobSequenceTest.rb'

# Physik aktivieren
$PM.setEnabled(true)

$SCRIPT.log("init map 'aitest'...")

$SCRIPT.log("Create hero")
$hero = $GOM.createGameObject("hero", 1);

$SCRIPT.log("Place hero into scene")
#$hero.setPosition([1.0, 0.0, 0.0]);
$hero.placeIntoScene()


$SCRIPT.log("Prepare hero for being the active character")
PlayerSettings.preparePlayer($hero)

$SCRIPT.log("Set hero as active character")
PartyManager.getSingleton().setActiveCharacter($hero)

$SCRIPT.log("Hero ready")

$SCRIPT.log("map 'aitest' initialisiert.");

$debugkeys = DebugKeys.new();
$IM.linkKeyToRubyCommand('G',"$debugkeys.toggleWP()");

$SCRIPT.log("debug keys are set up");

#DoorTest.new([0.0, 0.0, -5.0])

#WallSet1.new([0, 0, 0])
#WallSet2.new([0, 0, 0])

WayPointTest.new([0, 0, 0])

NpcTest.new([-5, 0, 5])
NpcCapsuleTest.new([0, 0, 5])

#TimeTest.new([-5, 0, -5])
#ErrorHandlingTest.new([-10, 0, 5])
#InventoryTest.new([0, 0, 10])
#JobSequenceTest.new([5, 0, -5])
$SCRIPT.log("tests initialisiert.");


