load "embed.rb"

require 'player.rb'
require 'hero.rb'
require 'DebugKeys.rb'

#require 'doortest.rb'

require 'WallSet1.rb'
require 'WallSet2.rb'
require 'WallSet3.rb'

#require 'NpcTest.rb'
require 'NpcCapsuleTest.rb'

#require 'WayPointTest.rb'
#require 'WayPointTest2.rb'

require 'AStarTest2.rb'
require 'AStarTest3.rb'
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
PartyManager.getSingleton().setActiveCharacter($hero)
$SCRIPT.log("Hero ready")

$SCRIPT.log("map 'regressiontest' initialisiert.");

$debugkeys = DebugKeys.new();
$IM.linkKeyToRubyCommand("G","$debugkeys.toggleWP()");

$SCRIPT.log("debug keys are set up");

#DoorTest.new([0.0, 0.0, -5.0])

#WallSet1.new([0, 0, 0])
#WallSet2.new([0, 0, 0])
WallSet3.new([0, 0, 0], 0.5, 20, [1,1,1,1])
#WallSet3.new([0, 0, 0], 0.5, 10, [1,1,0,1])
#WallSet3.new([0, 0, 0], 0.5, 1,  [0,0,0,1])


#NpcTest.new([-5, 0, 5])
#NpcCapsuleTest.new([0, 0, 5])

#$MyWPT = WayPointTest.new([0, 0, 0])
#$MyWPT.save()
#$MyWPT = WayPointTest2.new([0, 0, 0])
#$MyWPT.load()
#$MyAS = AStarTest1.new([0, 0, 0])
#$MyAS.create()
#$MyAS.initastar()
#$IM.linkKeyToRubyCommand('H',"$MyAS.calcFromTo([9.3,8.2,-10,-3])");

wolf = $GOM.createGameObject("Wolf")
wolf.placeIntoScene
wolf.setPosition [0.0,0.0,-5.0]

AStarTest3.new([0.0,0.0,-5.0])

$SCRIPT.log("tests initialisiert.");
