#==================================
#		Questbuch
#==================================
questBook = RulesSubsystem.getSingleton().getQuestBook()

hauptQuest = 
	Quest.new(
		"hauptquest",
		"Probleme",
		"Findet heraus was im Wald vor sich geht und was die Dorfbewohner beunruhigt  - und löst ihr Problem.")
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
		"hauptquest522",
		"Wolf-Belagerung",
		"Erledige die Wölfe, die den Turm belagern."))	
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest522").setPartsToDo(4)


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


aufenthaltsortQuest = 
	Quest.new(
		"aufenthaltsort",
		"Aufenhaltsort des alten Mannes aus dem Wald",
		"Bringt den genauen Aufenthaltsort des Alten Mannes im Wald in Erfahrung.")
questBook.addQuest(aufenthaltsortQuest)
RulesSubsystem.getSingleton().getQuestBook().getQuest("aufenthaltsort").setKnown(false)
RulesSubsystem.getSingleton().getQuestBook().getQuest("aufenthaltsort").setState(Quest::ASSIGNED)