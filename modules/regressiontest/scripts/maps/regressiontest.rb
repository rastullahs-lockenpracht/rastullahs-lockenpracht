load "embed.rb"

require 'player.rb'
require 'hero.rb'
require 'messagebox.rb'

# Physik aktivieren
$PM.setEnabled(true)

$SCRIPT.log("init map 'regressiontest'...")

$SCRIPT.log("Prepare hero for being the active character")
$hero = $GOM.getGameObject(10);
PlayerSettings.preparePlayer($hero)

$SCRIPT.log("Set hero as active character")
$UI.setActiveCharacter($hero)

$SCRIPT.log("Hero ready")

$SCRIPT.log("begin tests.")

require 'doortest.rb'
require 'effecttest.rb'
require 'ErrorHandlingTest.rb'
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

require 'WalkJobTest.rb'

errors = 0;
tests = 0;

tests += 1; errors += DoorTest.new([  0, 0, -5]).run()
tests += 1; errors += EffectTest.new([  5, 0, 5]).run()
tests += 1; errors += ErrorHandlingTest.new([-10, 0, 5]).run()
tests += 1; errors += GameObjectCreationTest.new([0, 0, -10]).run()
tests += 1; errors += InventoryTest.new([  0, 0, 10]).run()
tests += 1; errors += JobSequenceTest.new([  5, 0, -5]).run()
tests += 1; errors += LightzoneTest.new([-10, 0, -5]).run()
tests += 1; errors += MapLoadingTest.new([ -5, 0, -10]).run()
tests += 1; errors += MaterialSlotTest.new([-15, 0, -5]).run()
tests += 1; errors += MergeableMeshTest.new([ 10, 0, 0]).run()
tests += 1; errors += MeshAreaTest.new([-50, 0,-55]).run()
tests += 1; errors += NpcTest.new([ -5, 0, 5]).run()
tests += 1; errors += SelectorTest.new([  5, 0, 0]).run()
tests += 1; errors += SoundTest.new([ 10, 0, 10]).run()
tests += 1; errors += TimeTest.new([ -5, 0, -5]).run()
tests += 1; errors += WalkJobTest.new([  0, 0, -3]).run()

if errors > 0
  MessageBox.showModal(
	errors.to_s() + " of the " + tests.to_s() +  " tests were not successful.",
	"Regressiontest Results",
	MessageBox::OK)
end

$SCRIPT.log("tests initialisiert.")

$SCRIPT.log("map 'regressiontest' initialisiert.")
