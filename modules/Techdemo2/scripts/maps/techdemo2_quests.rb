questBook = RulesSubsystem.getSingleton().getQuestBook()

hauptQuest = 
	Quest.new(
		"hauptquest",
		"Probleme",
		"äöüÄÖÜFindet heraus was im Wald vor sich geht und die Dorfbewohner beunruhigt und löse das Problem.")
questBook.addQuest(hauptQuest)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest").setState(Quest::OPEN)

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest1",
		"Die Dorfbewohner",
		"Sucht Sonja, die Dorferste, und holt mehr Informationen ein."))

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest2",
		"Die Dorfbewohner",
		"Sprecht mit Jarn, Wolfram und Tsafried um mehr über das Problem der Dorfbewohner herauszufinden"))

quest = questBook.getQuest("hauptquest2")
quest.setPartsDone(quest.getPartsDone() + 3)

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest3",
		"Die Dorfbewohner",
		"Sprecht nun mit Sonja und berichtet ihr von Euren Ermittelungen."))

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest41",
		"Der alte Mann aus dem Wald",
		"Findet den Verrückten im Wald und macht ihn unschädlich. Bei Gegenwehr zögert nicht das Schwert einzusetzen."))

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest42",
		"Der alte Mann aus dem Wald",
		"Geht in den Wald und sichert die Gesundheit des Naturkundigen im Wald. Beschützt ihn vor den Raubtieren."))

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
		"Kehre ins Dorf zurück und berichte Sonja von deinen Erfolgen"))	



hauptQuest = 
	Quest.new(
		"wolfram",
		"Wolfram öffnet die Tür nicht",
		"Bringt Wolfram dazu, die Tür zu öffnen, damit Ihr mit ihm sprechen könnt.")

hauptQuest = 
	Quest.new(
		"paket",
		"Ein seltsames Paket",
		"Bringt Jarns Paket zu Wolfram, aber geht vorsichtig damit um!")

hauptQuest = 
	Quest.new(
		"aufenthaltsort",
		"Aufenhaltsort des alten Mannes aus dem Wald",
		"Findet den Aufenthaltsort des alten Mannes aus dem Wald.")



