$Core.log("Loading menu background..")
$Core.loadMap("Octree", "intro.mesh");
$Core.log("done")
$World = $Core.getWorld();
$Core.log("creating dummy player for menu.")
player = GameObject.new(10001, "Blah", "Blubb");
$Core.log("done.")
$Core.log("showMainMenu()..")
$Ui.showMainMenu(player);
$Core.log("done.")
