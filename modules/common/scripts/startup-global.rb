load 'embed.rb'
load 'global-settings.rb'

$CORE.log("Loading menu background..")
$CORE.loadMap("Octree", "intro.scene","intro.rb");
$CORE.log("done")
$World = $CORE.getWorld();

$CORE.log("creating dummy player for menu.")
player = GameObject.new(10001, "Blah", "Blubb");
# PlayerSettings.preparePlayer(player)
$CORE.log("done.")

GlobalSettings.addIngameActions(ActionManager.getSingleton())

$CORE.log("showMainMenu()..")
$UI.showMainMenu(player)
$CORE.log("done.")
