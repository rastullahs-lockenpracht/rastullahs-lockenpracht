# Nur eingebettet starten
load( "embed.rb" );

print( "Actor-Tests wird geladen" );

$World.setShowBoundingBoxes( true );

# Normales Erstellen & Einfügen
$CORE.log("Partikel Actor erstellen");
partikelDings = $AM.createParticleSystemActor("EinfachsePartikel", "PEExamples/ignifaxius" );
$CORE.log("Partikel Actor in die Szene einfügen");
partikelDings.placeIntoScene( 415.0, 5.0, -100.0, 1.0, 0.0, 0.0, 0.0);


# Ein Objekt an einen Bone anhängen
$CORE.log("BefestigungsMesh  Actor erstellen");
befestigDings = $AM.createMeshActor("BefestigungsMesh", "ins_fackel_01.mesh" );
$CORE.log("BefestigungsMesh2  Actor erstellen");
befestigDings2 = $AM.createMeshActor("BefestigungsMesh2", "ins_fackel_01.mesh" );

$CORE.log("HalteMesh Actor erstellen");
halteDings = $AM.createMeshActor("HalteMesh", "held.mesh" );
$CORE.log("HalteMesh Actor in die Szene einfügen");
halteDings.placeIntoScene( 315.0, 5.0, -100.0, 1.0, 0.0, 0.0, 0.0);

$CORE.log("Mesh an HalteMesh befestigen");
halteDings.attachToSlot( befestigDings, "Bone15", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 1.0, 0.0, 0.0 ], 90.0 );
$CORE.log("Mesh von HalteMesh abnehmen");
halteDings.detach( befestigDings );
$CORE.log("Mesh an HalteMesh erneut befestigen");
halteDings.attachToSlot( befestigDings, "Bone15", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 1.0, 0.0, 0.0 ], 90.0 );
$CORE.log("Mesh an BefestigungsMesh befestigen");
befestigDings.attachToSlot( befestigDings2, "SLOT_FAR_END", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 1.0, 0.0, 0.0 ], 120.0 );
$CORE.log("Animation abspielen");
halteDings.getControlledObject().startAnimation( "gehloop" )

# Objekte über Nodes verknüpfen
$CORE.log("BefestigungsPartikel Actor erstellen");
befestigDingsP = $AM.createParticleSystemActor("BefestigungsPartikel", "PEExamples/ignifaxius" );

$CORE.log("Leeren Actor erstellen");
leerDings = $AM.createEmptyActor("Leer");
$CORE.log("Leeren Actor in die Szene einfügen");
leerDings.placeIntoScene( 115.0, 5.0, -100.0, 1.0, 0.0, 0.0, 0.0);

$CORE.log("Partikel an Leeren Actor befestigen");
leerDings.attach( befestigDingsP );
$CORE.log("Partikel vom Leeren Actor abnehmen");
leerDings.detach( befestigDingsP );
$CORE.log("Partikel erneut an Leeren Actor befestigen");
leerDings.attach( befestigDingsP );
$CORE.log("Leeren Actor aus der Szene entfernen");
leerDings.removeFromScene();
$CORE.log("Leeren Actor wieder in die Szene einfügen");
leerDings.placeIntoScene( 115.0, 5.0, -100.0, 1.0, 0.0, 0.0, 0.0);

print( "Actor-Tests fertig geladen" );