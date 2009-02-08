load "embed.rb"
$World = $CORE.getWorld()
require 'player.rb'


$SCRIPT.log("Initialize world");
$PM.setEnabled(true);
$CORE.getWorld().setSkyBox(true, "rl/dsa07", 100.0);
$World.setFog( World::FOG_EXP, [0.658,0.7568,0.9137,1.0], 0.00049, 0.8, 4.0);

$SCRIPT.log("Initialize hero");
$hero = $GOM.getGameObject(1);
PlayerSettings.preparePlayer($hero);
PartyManager.getSingleton().setActiveCharacter($hero)
