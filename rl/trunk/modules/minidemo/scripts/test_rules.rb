load 'test_rules_actions.rb'

$gameTisch = GameObject.new(12, "Tisch", "Ein Tisch aus Holz. Sieht irgendwie komisch aus, vielleicht steckt mehr dahinter")
$gameTisch.addAction(HeiltrankWirkung.new("Beruehren", "Tisch anfassen", "A"))
chdesc = ChangeDescriptionAction.new("UntersuchungGelungen", "Dieser Tisch hat magische Heilfertigkeiten")
view = UseTalent.new("Untersuchen", "Tisch genauer ansehen", "Magiekunde", 5, chdesc)
$gameTisch.addAction(chdesc, 4) # ACT_DISABLED
$gameTisch.addAction(view)
#$UI.showActionChoice($gameTisch)
print "Oh, ein Tisch"
print "Beschreibung vorher: '"+$gameTisch.getDescription+"'"
print "Den guck ich mir doch mal genauer an (Anwendung von Magiekunde)"
view.doAction($gameTisch, $UI.getActiveCharacter(), $UI.getActiveCharacter())
print "Beschreibung nachher: '"+$gameTisch.getDescription+"'"
