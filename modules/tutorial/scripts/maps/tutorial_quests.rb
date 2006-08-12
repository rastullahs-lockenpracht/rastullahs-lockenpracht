load "embed.rb"

questBook = RulesSubsystem.getSingleton().getQuestBook()

hauptQuest = 
	Quest.new(
		"hauptquest",
		"Hauptquest",
		"Solange dieses Quest auf OPEN steht kann im im Dialog 5 eine Gesprächsoption auswählen.")
questBook.addQuest(hauptQuest)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest").setKnown(true)


partsQuest =
	Quest.new(
		"folterpunkte",
		"Folterpunkte",
		"Quest, das in 08 zum Punkte zählen benutzt wird.")
questBook.addQuest(partsQuest)

quest = questBook.getQuest("folterpunkte")
RulesSubsystem.getSingleton().getQuestBook().getQuest("folterpunkte").setPartsToDo(10)
RulesSubsystem.getSingleton().getQuestBook().getQuest("folterpunkte").setKnown(true)