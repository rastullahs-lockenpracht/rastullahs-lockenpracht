#==================================
#		Questbuch
#==================================
questBook = RulesSubsystem.getSingleton().getQuestBook()

hauptQuest = 
	Quest.new(
		"hauptquest",
		"Probleme",
		"Findet heraus was im Wald vor sich geht und die Dorfbewohner beunruhigt und löse das Problem.")
questBook.addQuest(hauptQuest)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest").setKnown(true)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest").setState(Quest::ASSIGNED)


hauptQuest.addSubquest(
	Quest.new(
		"hauptquest1",
		"Die Dorfbewohner",
		"Sucht Sonja, die Dorferste, und holt mehr Informationen ein."))
quest = questBook.getQuest("hauptquest1")
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest1").setKnown(true)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest1").setState(Quest::ASSIGNED)

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest2",
		"Die Dorfbewohner",
		"Sprecht mit Jarn, Wolfram und Tsafried um mehr über das Problem der Dorfbewohner herauszufinden"))

quest = questBook.getQuest("hauptquest2")
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest2").setPartsToDo(3)

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest3",
		"Die Dorfbewohner",
		"Sprecht nun mit Sonja und berichtet ihr von Euren Ermittelungen. Dies ist ein Unsinnsquest. Könnte eigentlich gelöscht werden."))

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest41",
		"Der alte Mann aus dem Wald",
		"Findet den Wald in dem sich der Verrückte Alt aufhält und macht diesen dort unschädlich. Bei Gegenwehr zögert nicht das Schwert einzusetzen."))

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest42",
		"Der alte Mann aus dem Wald",
		"Geht in den Wald um dort die Gesundheit des Naturkundigen zu sichern. Beschützt ihn vor den Raubtieren."))

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest51",
		"Der alte Mann aus dem Wald",
		"Findet den Alten Mann und macht ihn unschädlich."))

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest52",
		"Der alte Mann aus dem Wald",
		"Findet den Vermissten und macht eventuelle Gefahren für ihn und das Dorf unschädlich."))

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest6",
		"Der alte Mann aus dem Wald",
		"Kehre zurück zu Sonja und berichtet ihr von deinen Erfolgen"))	


wolframQuest = 
	Quest.new(
		"wolfram",
		"Wolfram öffnet die Tür nicht",
		"Bringt Wolfram dazu, die Tür zu öffnen, damit Ihr mit ihm sprechen könnt.")
questBook.addQuest(wolframQuest)

paketQuest = 
	Quest.new(
		"paket",
		"Ein seltsames Paket",
		"Bringt Jarns Paket zu Wolfram, aber geht vorsichtig damit um!")
questBook.addQuest(paketQuest)


TurmEingangQuest = 
	Quest.new(
		"TurmEingang",
		"TurmEingangs Zone",
		"Ein internes Quest statt globaler Variable")
questBook.addQuest(TurmEingangQuest)

aufenthaltsortQuest = 
	Quest.new(
		"aufenthaltsort",
		"Aufenhaltsort des alten Mannes aus dem Wald",
		"Bringt den genauen Aufenthaltsort des Alten Mannes im Wald in Erfahrung.")
questBook.addQuest(aufenthaltsortQuest)
RulesSubsystem.getSingleton().getQuestBook().getQuest("aufenthaltsort").setKnown(false)
RulesSubsystem.getSingleton().getQuestBook().getQuest("aufenthaltsort").setState(Quest::ASSIGNED)


#==================================
#		Tagebuch
#==================================

#Hauptquest
	questBook = RulesSubsystem.getSingleton().getQuestBook()
	questBook.addJournalEntry("Probleme mit Wilden Tieren I",
	"Gestern bin ich mit einem Problem in einem der umliegenden Dörfer betraut worden. In einer kleinen Siedlung zwischen Wandleth und Ruchin haben die Menschen wohl Probleme mit Raubtieren.Die Dorferste, Sonja heißt sie, kam vor einigen Tagen persönlich bei uns vorbei und meinte, dies sei ein Fall für uns Stadtwache.\
	Meine Instruktionen sind klar: In das Dorf gehen und den Menschen helfen ihre Probleme in den Griff zu kriegen. Hoffentlich verlangt man von mir nicht einen Bogen in die Hand zu nehmen und Tiere zu jagen.\
	Ich habe zudem aus Wandleht eine Heiler-Tasche mitbekommen. Wer weiß wozu sie alles gebraucht werden könnte.\
	Es ist jetzt Mittag und ich sehe die Häuser schon. Ich sollte wohl zuerst Sonja aufsuchen und genau herausfinden was eigentlich los ist.");

	questBook = RulesSubsystem.getSingleton().getQuestBook()
	questBook.addJournalEntry("Probleme mit Wilden Tieren II",
	"Ich habe mit Sonja geredet. Um mehr über die Sache zu erfahren soll ich mit einigen Dorfbewohnern reden. Ihr Mann, Jarn, wohnt in ihrem Haus. Wolfram, ein Dichter, wohnt in einem zweiten und ein gewisser Tsafried hält sich anscheinend gerne in einer kleinen Baumgruppe in der Nähe auf.\
	Wenn ich alle Informationen habe, die ich brauche soll ich wieder zu ihr zurückkehren und wir besprechen dann was zu tun ist. Scheinbar ist doch mehr an der Sache als einfach nur ein hungriges Tier.");

	questBook = RulesSubsystem.getSingleton().getQuestBook()
	questBook.addJournalEntry("Probleme mit Wilden Tieren III",
	"Ich habe mit allen dreien gesprochen. Das Bild fügt sich zusammen. Offenbar gibt es einen alten Mann, der im Wald lebt und manchmal mit den Dorfbewohnern Handel treibt. Dieser ist vor einer Woche verschwunden und niemand weiß wo er ist. Kurz danach haben die Übergriffe der Wölfe begonnen.\
	Während Wolfram sich nur sicher ist, dass der Mann aus dem Wald ganz sicher mit den Wölfen im Dorf zu tun hat, sich aber nicht festlegen will ob er nun auch Opfer oder ob er Verursacher ist ,sieht Jarn die Sache eindeutig: Der Mann aus dem Wald hetzt die Wölfe gegen das Dorf, weil neidisch auf dessen Leben ist, besonders auf seine Frau Sonja, die mit dem Alten Mann Handel treibt und sich in einer überlegenen Situation zu befinden scheint. Sie gibt ihm wohl weniger als er verdient hat. Tsafried hingegen ist genau der anderen Meinung. Er ist von der Unschuld des Mannes überzeugt. Dieser scheint so etwas wie ein Mentor oder Lehrer für ihn zu sein und Tsafried macht sich große Sorgen, dass etwas im Wald passiert ist, dass den Mann abhält ins Dorf zu kommen. Das scheint auch irgend etwas mit dem auftauchen der Wölfe zu tun zu haben. Tsafried bittet mich im Wald nachzusehen. Nichts anderes habe ich vor!\
	Ich sollte mit Sonja reden um weitere Schritte einzuleiten.");

	#Alter Mann Böse
		questBook = RulesSubsystem.getSingleton().getQuestBook()
		questBook.addJournalEntry("Probleme mit Wilden Tieren V",
		"Nun ist es raus: Der alte Mann aus dem Wald hetzt die Wölfe auf die Dorfbewohner! Allein diese Tatsache ist unheimlich genug und kann schon nicht mit den Zwölfen gefällen Dingen zugehen. Ich werde in den Wald gehen und ihn festsetzen, notfalls mit Gewalt. Dieser Verrückte ist eine Gefahr für die Öffentlichkeit. ");
		
	#Wolf Böse
		questBook = RulesSubsystem.getSingleton().getQuestBook()
		questBook.addJournalEntry("Der alte Mann aus dem Wald I",
		"Es ist eindeutig: Seit der alte Mann aus dem Wald hier nicht mehr gesehen wurde haben die Wolfübergriffe eingesetzt. Er muß sie vorher auf eine rechtschaffende Weise vom Dorf ferngehalten haben um die Bewohner zu schützen und nun ist etwas geschehen, was ihn davon abhält. Ich sollte in den Wald gehen und nach dem rechten sehen. Aber ich sollte vorsichtig sein, immerhin gibt es dort Wölfe, die nicht davor scheuen in ein Dorf voller Menschen zu kommen.");



#Nebenquests
	questBook = RulesSubsystem.getSingleton().getQuestBook()
	questBook.addJournalEntry("Wolfram öffnet die Tür nicht",
	"Ich war an Wolframs Tür. Er öffnet mir nicht, aber ich hörte, dass er zuhause ist! Ich muß ihn dazu bringen mir die Tür zu öffnen.");

	questBook = RulesSubsystem.getSingleton().getQuestBook()
	questBook.addJournalEntry("Ein seltsames Paket",
	"Ich habe mich einverstanden erklärt ein Paket von Jarn zu Wolfram zu bringen. Dieser wohnt gleich gegenüber. Ich frage mich was dort drinnen ist. Waren das gerade Geräusche aus dem Inneneren?");

	#Weg bekannt
	questBook = RulesSubsystem.getSingleton().getQuestBook()
	questBook.addJournalEntry("Aufenhaltsort des alten Mannes aus dem Wald",
	"Zuletzt wurde der Mann in der Nähe eines verlassenen Turms gefunden. Sonja hat mir den Weg erklärt, ich muß einfach nur einem Pfad außerhalb des Dorfes folgen.");