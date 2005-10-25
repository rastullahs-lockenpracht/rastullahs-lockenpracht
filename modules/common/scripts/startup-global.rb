load 'embed.rb'
load 'global-settings.rb'

# Der GC reisst oft die Objekte ab
GC.disable

$CORE.log("Loading menu background..")
$CORE.loadMap("Octree", "intro.scene", "General", "intro.rb");
$CORE.log("done")
$World = $CORE.getWorld();

GlobalSettings.addIngameActions(ActionManager.getSingleton())
$UI.update()
