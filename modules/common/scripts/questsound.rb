load "embed.rb"

class QuestSoundPlayer < QuestChangeListener 

	def initialize()
		p "erzeugt"
	end

	def questStateChanged( anEvent )
		p "State verändert"
		p anEvent.getQuest();
	end

	def questPartsDoneChanged( anEvent )
		p "Teile verändert"
		p anEvent.getQuest();
	end

	def questKnownChanged( anEvent )
		p "Sichtbarkeit verändert"
		p anEvent.getQuest();
	end

	def questSubquestAdded( anEvent )
		p "Subquest hinzugefügt"
		p anEvent.getQuest();
	end

end

var = QuestSoundPlayer.new();
questBook = RulesSubsystem.getSingleton().getQuestBook();
questBook.addQuestChangeListener( var  );
