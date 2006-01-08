load 'embed.rb'
load 'global-settings.rb'

$SCRIPT.log("Loading menu background..")
$CORE.loadMap("Octree", "intro.scene", "General", "intro.rb");
$SCRIPT.log("done")
$World = $CORE.getWorld();

GlobalSettings.addIngameActions(ActionManager.getSingleton())
$UI.update()
