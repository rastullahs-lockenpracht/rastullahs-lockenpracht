load 'embed.rb'
load 'global-settings.rb'

$CORE.log("Loading menu background..")
$CORE.loadMap("Octree", "intro.scene","intro.rb");
$CORE.log("done")
$World = $CORE.getWorld();

GlobalSettings.addIngameActions(ActionManager.getSingleton())
$UI.update()
