load "embed.rb"

require 'player.rb'
require 'hero.rb'
require 'DebugKeys.rb'

require 'doortest.rb'

require 'WallSet1.rb'
require 'WallSet2.rb'

require 'NpcTest.rb'
require 'NpcCapsuleTest.rb'

require 'WayPointTest.rb'
require 'WayPointTest2.rb'

# Physik aktivieren
$PM.setEnabled(true)

$SCRIPT.log("init map 'regressiontest'...")

$SCRIPT.log("Create hero")
$hero = $GOM.createGameObjectProxy("hero", 1);

$SCRIPT.log("Place hero into scene")
#$hero.getGameObject().setPosition([0.0, 1.0, 0.0]);
$hero.getGameObject().placeIntoScene()


$SCRIPT.log("Prepare hero for being the active character")
PlayerSettings.preparePlayer($hero.getGameObject())

$SCRIPT.log("Set hero as active character")
$UI.setActiveCharacter($hero.getGameObject())

$SCRIPT.log("Hero ready")

$SCRIPT.log("map 'regressiontest' initialisiert.");

$debugkeys = DebugKeys.new();
$UI.linkKeyToRubyCommand('G',"$debugkeys.toggleWP()");

$SCRIPT.log("debug keys are set up");

#DoorTest.new([0.0, 0.0, -5.0])

#WallSet1.new([0, 0, 0])
WallSet2.new([0, 0, 0])

NpcTest.new([-5, 0, 5])
NpcCapsuleTest.new([0, 0, 5])

#$MyWPT = WayPointTest.new([0, 0, 0])
#$MyWPT.save()
$MyWPT = WayPointTest2.new([0, 0, 0])
$MyWPT.load()

$SCRIPT.log("tests initialisiert.");
