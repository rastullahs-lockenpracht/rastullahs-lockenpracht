load "embed.rb"

require 'player.rb'
require 'hero.rb'
require 'MapChange.rb'

$SCRIPT.log("Prepare hero for being the active character")
$hero = $GOM.getGameObject(10);
PlayerSettings.preparePlayer($hero)

$SCRIPT.log("Set hero as active character")
$UI.setActiveCharacter($hero)

$SCRIPT.log("Hero ready")