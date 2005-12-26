load 'embed.rb'
load 'global-settings.rb'

# Der GC reisst oft die Objekte ab
GC.disable

$SCRIPT.log("Loading menu background..")
$CORE.loadMap("Octree", "intro.scene", "General", "intro.rb");
$SCRIPT.log("done")
$World = $CORE.getWorld();

GlobalSettings.addIngameActions(ActionManager.getSingleton())
$UI.update()
