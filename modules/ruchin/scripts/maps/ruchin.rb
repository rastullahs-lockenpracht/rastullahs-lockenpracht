load "embed.rb"
$World = $CORE.getWorld()
require 'player.rb'
require 'mckhero.rb'
require 'npc.rb'


$SCRIPT.log("Initialize world");
loader = MapLoader.new("ruchin");
loader.loadMap("ruchin_lightzones.rlmap.xml");
$PM.setEnabled(true);
$CORE.getWorld().setSkyBox(true, "rl/dsa07", 100.0);
$World.setFog( World::FOG_EXP, [0.658,0.7568,0.9137,1.0], 0.00049, 0.8, 4.0);

$SCRIPT.log("Initialize hero");
$hero = $GOM.createGameObject("mckhero", 1);
$hero.placeIntoScene();
$hero.setPosition([ -35.3738746643066, -5.64282417297363, -64.6520767211914]);
$hero.setOrientation([ [ -4.43614335381426e-005, -0.999999940395355, -3.53998184436932e-005 ] , 67.3891525268555]);
PlayerSettings.preparePlayer($hero);
$UI.setActiveCharacter($hero)

$SCRIPT.log("Initialize NPCs");
$SCRIPT.log("Hesindiane...");
Hesindiane = $GOM.createGameObject("Hesindiane", 2);
Hesindiane.placeIntoScene();
Hesindiane.setPosition([ 123.848808288574, 0.264747649431229, -46.4587249755859]);
Hesindiane.setOrientation([ [ -1.40737538458779e-005, 1.0, -0.000235967105254531 ] , 230.07991027832]);
HesindianeBot = $AI.loadBot("Hesindiane", "nscs.xml");
HesindianeBot.setNonPlayerCharacter(Hesindiane);
#Hesindiane.addAction(TalkAction.new("Hesindiane", "nscs.xml");
