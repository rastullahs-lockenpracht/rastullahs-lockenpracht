# das fiese Wölfilein nach draußen setzen

#Model schonmal reinladen mit was anderem als dem Wolf, Spinne z.B.


#Tagebucheinträge bei Tötung einstellen
	questBook = RulesSubsystem.getSingleton().getQuestBook()
	questBook.addJournalEntry("Der alte Mann aus dem Wald II",
	"Geschafft! Ein großer Wolf hatte den Vermissten angefallen und ihn im Turm festgesetzt, wo ich ihn verwundet und fiebrig fand. Ich musste den Wolf töten und habe den alten Mann notdürftig versorgt. Ich sollte nun ins Dorf zurückkehren und alles berichten, die Dorfbewohner können sich dann um alles weitere kümmern, nun wo der Weg wieder frei ist.");

#Waffe wegstecken wenn Wolfi tot ist
# $AM.getActor("Held").detachWeapon();  

#am Ende sonja2.rb laden


