require 'globals.rb'
require 'player.rb'
require 'hero.rb'

$PM.setEnabled(true)

$World = $CORE.getWorld();
$World.setSkyBox(true, "rl/dsa07");

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

$CORE.log("Tageslicht erstellen..");
sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
sunlight.getControlledObject().setDirection(0.8, -2.0, -3.0);
$CORE.log("Tageslicht erstellt.");

# Richtungsvektor (x, y, z) des Lichtes
# Farbe auf Blassgelb setzen
# sunlight.setDiffuseColour(0.7, 0.7, 0.4);

$CORE.log("Held erstellen");
hero = Hero.new;
$CORE.log("Held erstellt");
$CORE.log("Held in die Szene einfuegen.");
hero.getActor().placeIntoScene([-530.0, 420.0, -1020.0], [1.0, 0.0, 1.0, 0.0]);
# hero.getActor().yaw(180.0)
$CORE.log("Held eingefügt.");
PlayerSettings.preparePlayer(hero);
$CORE.log("Held vorbereitet.");
$UI.setActiveCharacter(hero);
$CORE.log("Held als aktiver Charakter gesetzt.");

load "waldspinne.rb"
waldspinne = Waldspinne.new
waldspinne.getActor().placeIntoScene(-1500.0, -135.0, 1020.0, 1.0, 0.0, 0.0, 0.0);	
waldspinne.getActor().setScale(5.0, 5.0, 5.0)
$GameEveMgr.addSphereAreaListener( waldspinne.getActor(), 500.0, CombatTrigger.new(), Actor::QGF_PLAYER )

held = $AM.getActor( "Held" );
held.setQueryMask( Actor::QGF_PLAYER );