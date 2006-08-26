load "embed.rb"

require 'player.rb'
require 'hero.rb'

# Physik aktivieren
$PM.setEnabled(true);

$SCRIPT.log("init map 'regressiontest'...");
$World = $CORE.getWorld()

$hero = Hero.new;
$hero.getActor().placeIntoScene([0.0, 0.2, 0.4]);
PlayerSettings.preparePlayer($hero);
$UI.setActiveCharacter($hero);

$SCRIPT.log("map 'regressiontest' initialisiert.");
