require 'globals.rb'
require 'player.rb'
require 'hero_scaled.rb'

$PM.setEnabled(true)

$World = $CORE.getWorld();
$World.setSkyBox(true, "rl/dsa07", 5000.0);

# Farbe/Intensität des Umgebungslichtes.
# Ohne Berücksichtigung von Lichtquellen.
$World.setAmbientLight(0.4, 0.4, 0.4);
# Voellige Finsternis
# $World.setAmbientLight(0.0, 0.0, 0.0);

#   createLightActor(eindeutiger name, Lichttyp
#   Zweites Argument ist der Lichttyp
#   0: Punktlicht. Licht wird von der Lichtposition aus in
#      alle Richtungen gestrahlt.
#   1: Directional Light. Licht wird gleichmäßig in eine
#      Richtung ausgestrahlt (Wie z.B. Licht von der Sonne)
#
#   2: Spotlight. Ein Lichtkegel, der einen Lichtkegel in
#      eine bestimmte Richtung ausstrahlt.

$SCRIPT.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(0.8, -2.0, -3.0);
$SCRIPT.log("Tageslicht erstellt.");

# Richtungsvektor (x, y, z) des Lichtes
# Farbe auf Blassgelb setzen
# sunlight.setDiffuseColour(0.7, 0.7, 0.4);

$SCRIPT.log("Held erstellen");
hero = Hero.new;
$SCRIPT.log("Held erstellt");
$SCRIPT.log("Held in die Szene einfuegen.");
hero.getActor().placeIntoScene([-5.300, 4.200, -10.200], [1.0, 0.0, 1.0, 0.0]);
# hero.getActor().yaw(180.0)
$SCRIPT.log("Held eingefügt.");
PlayerSettings.preparePlayer(hero);
$SCRIPT.log("Held vorbereitet.");
$UI.setActiveCharacter(hero);
$SCRIPT.log("Held als aktiver Charakter gesetzt.");

#load "waldspinne.rb"
#waldspinne = Waldspinne.new
#waldspinne.getActor().placeIntoScene(-1500.0, -135.0, 1020.0, 1.0, 0.0, 0.0, 0.0);	
#waldspinne.getActor().setScale(5.0, 5.0, 5.0)
#$GameEveMgr.addSphereAreaListener( waldspinne.getActor(), 500.0, CombatTrigger.new(), Actor::QGF_PLAYER )

held = $AM.getActor( "Held" );
held.setQueryMask( Actor::QGF_PLAYER );