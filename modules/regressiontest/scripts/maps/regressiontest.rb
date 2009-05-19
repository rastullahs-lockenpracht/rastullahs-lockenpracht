load "embed.rb"

require 'player.rb'
require 'hero.rb'
require 'messagebox.rb'

# Physik aktivieren
$PM.setEnabled(true)

$SCRIPT.log("init map 'regressiontest'...")

$SCRIPT.log("Prepare hero for being the active character")
$hero = $GOM.getGameObject(10);
$hero.modifyLe(-28);
PlayerSettings.preparePlayer($hero)

$SCRIPT.log("Set hero as active character")
PartyManager.getSingleton().setActiveCharacter($hero)

$SCRIPT.log("Hero ready")

$SCRIPT.log("begin tests.")

require 'doortest.rb'
require 'effecttest.rb'
require 'ErrorHandlingTest.rb'
require 'FetchItemJobTest.rb'
require 'GameObjectCreationTest.rb'
require 'InventoryTest.rb'
require 'JobSequenceTest.rb'
require 'LightzoneTest.rb'
require 'MapLoadingTest.rb'
require 'MaterialSlotTest.rb'
require 'MergeableMeshTest.rb'
require 'MeshAreaTest.rb'
require 'NpcTest.rb'
require 'SelectorTest.rb'
require 'SoundTest.rb'
require 'TimeTest.rb'
require 'CombatTest.rb'
require 'WalkJobTest.rb'
require 'GoToJobTest.rb'
require 'CharacterSelectionTest.rb'
require 'CollisionsCacheTest.rb'

class RegressionTestSuite
	def initialize
		@mErrors = 0
		@mTests = 0
		@mFailedTests = Array.new()
	end

	def runTest(test, coords)
		p "Start test " + test.to_s()
		@mTests += 1
		testErrors = test.new(coords).run()
		if testErrors != 0
			@mErrors += testErrors
			@mFailedTests << test.to_s()
		end
		p "Finished test " + test.to_s()
	end
	
	def runTests()
		runTest(DoorTest, [  0, 0, -5])
		runTest(EffectTest, [  5, 0, 5])
		runTest(ErrorHandlingTest, [-10, 0, 5])
		runTest(GameObjectCreationTest, [0, 0, -10])
		runTest(InventoryTest, [  0, 0, 10])
		runTest(JobSequenceTest, [  5, 0, -5])
		runTest(LightzoneTest, [-10, 0, -5])
		runTest(MapLoadingTest, [ -5, 0, -10])
		runTest(MaterialSlotTest, [-15, 0, -5])
		runTest(MergeableMeshTest, [ 10, 0, 0])
		runTest(MeshAreaTest, [-50, 0,-55])
		runTest(NpcTest, [ -5, 0, 5])
		runTest(SelectorTest, [  5, 0, 0])
		runTest(SoundTest, [ 10, 0, 10])
		runTest(TimeTest, [ -5, 0, -5])
		runTest(WalkJobTest, [  0, 0, -3])
        runTest(GoToJobTest, [ 0, 0, -30])
		runTest(CombatTest, [15, 0, 15])
  	runTest(FetchItemJobTest, [-15, 0, 15])
   runTest(CharacterSelectionTest, [-15, 0, -15])
   runTest(CollisionsCacheTest, [62, 0, -10])

		if @mErrors > 0
		  MessageBox.showModal(
			@mErrors.to_s() + " of the " + @mTests.to_s() +  " tests could not be initialized successfully.\nNot working:\n" + @mFailedTests.join(", "),
			"Regressiontest Results",
			MessageBox::OK)
		end

		$SCRIPT.log("tests initialisiert.")
	end
end

RegressionTestSuite.new().runTests()
$SCRIPT.log("map 'regressiontest' initialisiert.")
