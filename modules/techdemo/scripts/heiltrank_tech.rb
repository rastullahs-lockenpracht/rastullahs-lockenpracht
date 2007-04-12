# Nur eingebettet starten
load( "embed.rb" );

class HealAction < Action
    def initialize(name, description, quality, customLep, customOverMax)
       super(name, description)
       @mQuality = quality
       @mLep = customLep
       @mOverMax = customOverMax
    end

    def getTargetClass
       return TC_SELF;
    end

    def doAction(potion, user, target)
       targetActor = user.getActor();
       gulpActor = targetActor.getChildByName(targetActor.getName()+"_schlucken");
       gulpActor.getControlledObject().play();
	RulesSubsystem.getSingleton().getQuestBook().getQuest("hoehleZeugTrank").setState(Quest::COMPLETED)
	
       overMax = false
       case @mQuality
            when "M"
               lep = -$DM.roll(3, 0)
            when "A"
               lep = $DM.roll(1, 0) # KL-1, GE-1 f�r 1h
            when "B"
               lep = $DM.roll(2, 0)
            when "C"
               lep = 10
            when "D"
               lep = $DM.roll(4, 0)
            when "E"
               lep = 1000000
            when "F"
               lep = 50 # Fuer eine Woche immun gegen magische Traenke und Gifte
            when "Custom"
               lep = @mLep
               overMax = @mOverMax
       end
       user.modifyLe(lep)
    end
end

class Heiltrank < Item
  
  def initialize(id)
    super(id)
  end
  
  def setProperty(key, value)
    if (key == "quality")
    	addAction(HealAction.new("drink", "Trinken", value, 0, 0))
    else
    	super(key, value)
    end
  end
end
