load "embed.rb"

class QuestSoundTimer < TimerListener
    def initialize(questSoundPlayer)
        super()
        @mQuestSoundPlayer = questSoundPlayer
    end

    def timerFired(event)
	    @mQuestSoundPlayer.playSound() 
    end
end

class QuestSoundPlayer < QuestListener 
	def initialize()
		super()
		@mHasSound = false;
		@mSound = nil;
		@mSoundTimer = TimerEventSource.new($CORE.getClock() + 1000, 1000)
        @mTimerListener = QuestSoundTimer.new(self)
		@mSoundTimer.addTimerListener( @mTimerListener )
	end

	def questStateChanged( anEvent )
		#p "State verändert"
		@mHasSound = true
	end

	def questPartsDoneChanged( anEvent )
		#p "Teile verändert"
		@mHasSound = true
	end

	def questKnownChanged( anEvent )
		#p "Sichtbarkeit verändert"
		@mHasSound = true;
	end

	def questSubquestAdded( anEvent )
		#p "Subquest hinzugefügt"
		@mHasSound = true
	end

	def journalEntryAdded( anEvent )
		# Journaleintrag hinzugefügt
		@mHasSound = true
	end
    
	def playSound()
        begin 
            if( @mHasSound )
                if( @mSound.nil? )
                    @mSound = $AM.createSoundSampleActor( "QuestAenderung", "quest_aenderung_gekritzel_01.ogg" )
                    
                    @mSound.getControlledObject().set3d(false)
                    @mSound.getControlledObject().load()
                end

                @mSound.getControlledObject().play()
                @mHasSound = false
		    end
        rescue 
            @mSoundTimer.removeTimerListener( @mTimerListener )
            @mHasSound = false
            p "Questsound ist kaputt gegangen"
            p @mSound
            p "@mSound.nil? " + (@mSound.nil?).to_s
            p "@mSound == nil " + (@mSound == nil).to_s
            p "@mSound.is_a? Actor " + (@mSound.is_a? Actor).to_s
        end
	end
end

RulesSubsystem.getSingleton().getQuestBook().addQuestListener(  QuestSoundPlayer.new()  )
