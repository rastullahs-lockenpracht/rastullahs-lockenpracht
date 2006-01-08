questBook = RulesSubsystem.getSingleton().getQuestBook()

techdemoQuest = 
	Quest.new(
		"techdemo",
		"Rastullahs Lockenpracht Techdemo I",
		"Seltsam hier alles... ich sollte mal mit jemandem reden (>u< drücken)")
questBook.addQuest(techdemoQuest)
RulesSubsystem.getSingleton().getQuestBook().getQuest("techdemo").setState(Quest::OPEN)

techdemoQuest.addSubquest(
	Quest.new(
		"techdemoEverquest",
		"Rastullahs Lockenpracht Techdemo II",
		"Arnolf Großschädel hat es mir gesagt. Ich kann ihn jederzeit wieder drauf ansprechen. Ich glaube, dieses Quest ist ohne meine Hilfe unlösbar! Ich muß ihnen helfen!"))

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

	hoehlenZeugQuest = Quest.new(
		"hoehleZeug",
		"In der Höhle umsehen",
		"Ich sollte mich in der Höhle umsehen und verschiedene Sachen ausprobieren.")

	hoehlenQuest.addSubquest( hoehlenZeugQuest )

		  hoehlenZeugQuest.addSubquest(
			Quest.new(
				"hoehleZeugFackel",
				"Fackel in die Hand nehmen",
				"Die Fackel finden und in die Hand nehmen"))
				
		  hoehlenZeugQuest.addSubquest(
			Quest.new(
				"hoehleZeugHebel",
				"Hebel umlegen",
				"Den Hebel beliebig umlegen"))
				
		  hoehlenZeugQuest.addSubquest(
			Quest.new(
				"hoehleZeugTrank",
				"Trank trinken",
				"Den Trank in der Höhle finden und ihn trinken. Ist klein, mit Mausmodus auswählen klappt besser als ihn so anzuvisieren."))