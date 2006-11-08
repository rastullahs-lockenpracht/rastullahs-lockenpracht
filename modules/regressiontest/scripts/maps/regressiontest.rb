load "embed.rb"

require 'player.rb'
require 'hero.rb'
require 'doortest.rb'

# Physik aktivieren
$PM.setEnabled(true)

$SCRIPT.log("init map 'regressiontest'...")
$World = $CORE.getWorld()

$hero = Hero.new
$hero.getActor().placeIntoScene([0.0, 0.2, 0.0])
PlayerSettings.preparePlayer($hero)
$UI.setActiveCharacter($hero)

DoorTest.new([0.0, 0.0, -5.0])

$SCRIPT.log("map 'regressiontest' initialisiert.")
