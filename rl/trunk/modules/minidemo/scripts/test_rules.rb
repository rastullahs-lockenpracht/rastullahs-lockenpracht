load 'test_rules_actions.rb'

$gameTisch = GameObject.new(12, "Tisch", "Ein Tisch aus Holz. Sieht irgendwie komisch aus, vielleicht steckt mehr dahinter");
$gameTisch.addAction(HeiltrankWirkung.new("Berühren", "Tisch anfassen", "A"));
chdesc = ChangeDescriptionAction.new("UntersuchungGelungen", "Dieser Tisch hat magische Heilfertigkeiten")
$gameTisch.addAction(chdesc, 4) # ACT_DISABLED
$gameTisch.addAction(UseTalent.new("Untersuchen", "Tisch genauer ansehen", "Magiekunde", 5, chdesc));
p $gameTisch
$Ui.showActionChoice($gameTisch);
p $gameTisch
