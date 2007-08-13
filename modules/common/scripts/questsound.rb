require 'jobs/soundjobs.rb'

class QuestSoundPlayer < QuestListener 
	def initialize()
		super()
        @lastPlayed = 0
	end

	def questStateChanged( anEvent )
        playSound()
	end

	def questPartsDoneChanged( anEvent )
        playSound()
	end

	def questKnownChanged( anEvent )
        playSound()
	end

	def questSubquestAdded( anEvent )
        playSound()
	end

	def journalEntryAdded( anEvent )
        playSound()
	end
    
	def playSound()
        now = TimeSourceManager.getSingleton().getTimeSource(TimeSource::GAMETIME).getClock()
        # only play sound, if we didn't play it in the last two seconds
        if (now - @lastPlayed) > 2000
            PlaySound2d("quest_aenderung_gekritzel_01.ogg")
            @lastPlayed = now
        end
 	end
end

RulesSubsystem.getSingleton().getQuestBook().addQuestListener(QuestSoundPlayer.new())
