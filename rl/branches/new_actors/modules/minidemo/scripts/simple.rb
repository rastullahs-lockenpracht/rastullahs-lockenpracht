# Dies sind Kommentare

# Nur eingebettet starten
load( "embed.rb" );

# print wird auf die Konsole umgeleitet
print( "print wird auf die Konsole umgeleitet" );

# Wichtige Komponenten
# $Core -> CoreSubsystem
# $World -> World
# $AF -> ActorFactory


# die Kamera holen
cam = $World.getCamera();

# TypNamen ausgeben
print( cam.getTypeName() );
# Kamera verschieben
cam.translate( 20.0, 20.0, 20.0 );

# Spiel Akteur erstellen
$gActor = $AF.createGameActor("Tisch","tisch_taverne.mesh");
# und positionieren
$gActor.setPosition(-128.0,128.0,16.0);
$gActor.rotate( 1.0, 0.0, 0.0, 90.0);

print $gActor.getName();

# Screenshot bitte
# $Core.makeScreenshot( "Rastullah" );

# Map laden
# $World.loadScene( "testraum.bsp" );



