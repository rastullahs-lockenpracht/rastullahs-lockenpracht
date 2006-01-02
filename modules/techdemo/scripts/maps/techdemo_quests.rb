questBook = RulesSubsystem.getSingleton().getQuestBook()

spinneQuest = 
	Quest.new(
		"spinne",
		"Eine Unterkunft und die Spinnen",
		"Um beim Bauern unterzukommen muss ich eine Spinne auf der anderen Seite des Flusses töten.")
questBook.addQuest(spinneQuest)

hoehlenQuest = 
	Quest.new(
		"hoehle", 
		"Die Räuberhöhle", 
		"In einer Höhle in den Bergen, die einst von Räubern als Unterschlupf benutzt wurde, liegen noch verborgene Schätze.")
questBook.addQuest(hoehlenQuest)

hoehlenQuest.addSubquest(
	Quest.new(
		"hoehleEingang",
		"Den Eingang finden",
		"Als erstes muss ich den Eingang finden. Er ist eventuell verschüttet."))

hoehlenQuest.addSubquest(
	Quest.new(
		"hoehleZeug",
		"In der Höhle Umsehen",
		"Ich sollte mich in der Höhle umsehen und verschiedene Sachen ausprobieren."))

RulesSubsystem.getSingleton().getQuestBook().getQuest("hoehleZeug").setPartsToDo(3)

