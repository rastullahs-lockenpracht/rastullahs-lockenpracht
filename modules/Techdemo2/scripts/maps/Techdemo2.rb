load "embed.rb"

require 'player.rb'
require 'hero.rb'

# Physik aktivieren
$PM.setEnabled(true);

$SCRIPT.log("init map 'Techdemo2'...");
$World = $CORE.getWorld()
$World.setSkyBox(true, "rl/dsa07", 100.0)
$SCRIPT.log("skybox set");

$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(0.7, -2.0, 0.8);
sunlight.getControlledObject().setCastShadows(false);
sunlight.getControlledObject().setDiffuseColour(0.9,0.9,0.8);
$SCRIPT.log("Tageslicht erstellt.");

$SCRIPT.log("Held erstellen");
$hero = Hero.new;
$SCRIPT.log("Held erstellt");
$SCRIPT.log("Held in die Szene einfuegen.");
$hero.getActor().placeIntoScene(0.0, 0.0, 13.0, 1.0, 0.0, 0.0, 0.0);
$SCRIPT.log("Held eingefügt.");
PlayerSettings.preparePlayer($hero);
$SCRIPT.log("Held vorbereitet.");
$UI.setActiveCharacter($hero);
$SCRIPT.log("Held als aktiver Charakter gesetzt.");

$World.setFog( World::FOG_EXP, [0.8,0.8,1.0,0.5], 0.00103, 0.4, 1.0);

$SCRIPT.log("map 'Techdemo2' initialisiert.");
$WF.toggleCharacterStateWindow()
