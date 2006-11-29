#==================================
#		Questbuch
#==================================
questBook = RulesSubsystem.getSingleton().getQuestBook()

hauptQuest = 
	Quest.new(
		"hauptquest",
		"Problems",
		"Find out what's happening in the forest and troubling the villagers and solve their problem.")
questBook.addQuest(hauptQuest)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest").setKnown(true)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest").setState(Quest::ASSIGNED)


hauptQuest.addSubquest(
	Quest.new(
		"hauptquest1",
		"The Villagers",
		"Look for Sonja the mayor of the village and get some more information from her."))
quest = questBook.getQuest("hauptquest1")
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest1").setKnown(true)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest1").setState(Quest::ASSIGNED)

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest2",
		"The Villagers",
		"Talk to Jarn, Wolfram and Tsafried to find out more about the villagers' problem."))

quest = questBook.getQuest("hauptquest2")
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest2").setPartsToDo(3)

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest3",
		"The Villagers",
		"Now talk to Sonja and tell her about your investigation. Dies ist ein Unsinnsquest. Könnte eigentlich gelöscht werden."))

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest41",
		"The Old Man in the Forest",
		"Locate the forest and find the old madman and take care of him. Don't hesitate to use your sword if there is any resistance."))

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest42",
		"The Old Man in the Forest",
		"Go into the forest and save the old man's health. Protect him from the predators."))

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest51",
		"The Old Man in the Forest",
		"Find the old madman and take care of him."))

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest52",
		"The Old Man in the Forest",
		"Find the lost man and take care of the things endangering him and the village."))

hauptQuest.addSubquest(
	Quest.new(
		"hauptquest522",
		"Wolf Siege",
		"Handle the wolfs sieging the tower."))	
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest522").setPartsToDo(4)


hauptQuest.addSubquest(
	Quest.new(
		"hauptquest6",
		"The Old Man in the Forest",
		"Go back to Sonja and tell her of your success."))	

wolframQuest = 
	Quest.new(
		"wolfram",
		"Wolfram Doesn't Open The Door",
		"Get Wolfram to open his door so you can talk to him.")
questBook.addQuest(wolframQuest)

paketQuest = 
	Quest.new(
		"paket",
		"A Strange Package",
		"Deliver Jarn's Package to Wolfram. Handle it carefully.")
questBook.addQuest(paketQuest)


aufenthaltsortQuest = 
	Quest.new(
		"aufenthaltsort",
		"Location of the Old Man in the Forest",
		"Locate the forest and find the old man.")
questBook.addQuest(aufenthaltsortQuest)
RulesSubsystem.getSingleton().getQuestBook().getQuest("aufenthaltsort").setKnown(false)
RulesSubsystem.getSingleton().getQuestBook().getQuest("aufenthaltsort").setState(Quest::ASSIGNED)