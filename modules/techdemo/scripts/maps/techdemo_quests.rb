questBook = RulesSubsystem.getSingleton().getQuestBook()

unterkunftQuest = 
	Quest.new(
		"unterkunft",
		"Eine Unterkunft",
		"Ich brauche eine Unterkunft für die Nacht, vielleicht kann ich in dem Haus oder im Schuppen unterkommen.")
questBook.addQuest(unterkunftQuest)
unterkunftQuest.setState(Quest::OPEN)

unterkunftQuest.add(
	Quest.new(
		"spinne", 
		"Die Riesenspinne", 
		"Auf der anderen Seite des Flusses lebt eine riesige Waldspinne. Arnolf Großschädel möchte, dass diese Spinne getötet wird. Erst danach bekomme ich ein Zimmer."))
unterkunftQuest.add(
	Quest.new(
		"unterkunft_reden",
		"Nochmal nach der Unterkunft fragen",
		"Jetzt, da die Spinne tot ist, sollte doch einem trockenen Plätzchen nichts mehr im Wege stehen."))

hoehlenQuest = 
	Quest.new(
		"hoehle", 
		"Die Räuberhöhle", 
		"In einer Höhle hinter dem Haus soll es eine Höhle geben, die einst von Räubern als Unterschlupf benutzt wurde. Vielleicht liegen dort noch verborgene Schätze.")
questBook.addQuest(hoehlenQuest)

hoehlenQuest.addSubquest(
	Quest.new(
		"hoehle_finden",
		"Den Eingang finden",
		"Als erstes muss ich den Eingang finden."))

hoehlenQuest.addSubquest(
	Quest.new(
		"hoehle_truhe",
		"Den Schatz finden",
		"Irgendetwas vom Schatz muss doch noch zu finden sein."))

