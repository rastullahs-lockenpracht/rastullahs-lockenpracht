load "embed.rb"

require 'player.rb'
require 'hero.rb'

# Physik aktivieren
$PM.setEnabled(true);

$World.setSkyBox(true, "rl/desert07", 100.0)

shadowlight = $AM.createLightActor("shadowlight", LightObject::LT_POINT);
shadowlight.placeIntoScene([0.0, 4.14, 0.0]);
shadowlight.getControlledObject().setCastShadows(true);
shadowlight.getControlledObject().setDiffuseColour(0.8,0.8,0.8);

$SCRIPT.log("Held erstellen");
$hero = Hero.new;
$SCRIPT.log("Held erstellt");
$SCRIPT.log("Held in die Szene einfuegen.");#-22.0130577087402, -6.70670890808105, 25.1438484191895
$hero.getActor().placeIntoScene([0.0, 0.0, 0.0]);
$SCRIPT.log("Held eingefügt.");
PlayerSettings.preparePlayer($hero);
$SCRIPT.log("Held vorbereitet.");
$UI.setActiveCharacter($hero);
$SCRIPT.log("Held als aktiver Charakter gesetzt.");
$heroVehicle = $AI.createAgent(AgentManager::AGENT_PLAYER, $hero);	
$SCRIPT.log("SteeringVehicle für Held erstellen...");