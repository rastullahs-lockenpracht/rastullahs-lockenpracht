$CORE.log("Loading menu background..")
$CORE.loadMap("Octree", "intro.scene");
$CORE.log("done")
$World = $CORE.getWorld();
$CORE.log("creating dummy player for menu.")
player = GameObject.new(10001, "Blah", "Blubb");
$CORE.log("done.")
$CORE.log("showMainMenu()..")
$UI.showMainMenu(player);
$CORE.log("done.")
