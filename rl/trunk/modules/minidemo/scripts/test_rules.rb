load 'test_rules_actions.rb'

$gameTisch = GameObject.new(12, "Tisch", "Ein Tisch aus Holz. Sieht irgendwie komisch aus, vielleicht steckt mehr dahinter")
$gameTisch.addAction(HeiltrankWirkung.new("Beruehren", "Tisch anfassen", "A"))
chdesc = ChangeDescriptionAction.new("UntersuchungGelungen", "Dieser Tisch hat magische Heilfertigkeiten")
view = UseTalent.new("Untersuchen", "Tisch genauer ansehen", "Magiekunde", 5, chdesc)
$gameTisch.addAction(chdesc, 4) # ACT_DISABLED
$gameTisch.addAction(view)
p "Beschreibung vorher: '"+$gameTisch.getDescription+"'"
$Ui.showActionChoice($gameTisch)
#view.doAction($gameTisch, $Ui.getActiveCharacter(), $Ui.getActiveCharacter())
p "Beschreibung nachher: '"+$gameTisch.getDescription+"'"
