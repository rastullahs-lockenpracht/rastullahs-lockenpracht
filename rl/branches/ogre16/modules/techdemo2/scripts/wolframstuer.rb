# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );
load( "door.rb" );

class TurTalkAction < TalkAction
    def doAction(object, actor, target)
        if (RulesSubsystem.getSingleton().getQuestBook().getQuest("wolfram").getState() != Quest::SUCCEEDED) 
            super(object, actor, target)
	    end
    end

    def canDo(enemy, user)    
        return RulesSubsystem.getSingleton().getQuestBook().getQuest("wolfram").getState() != Quest::SUCCEEDED
    end
end

class WolframsTuer < GameObject
    def getName()
       return "Wolframs Tür"
    end
    
    def getDialogfile()
       return "wolframstuer.xml"
    end
end


print( "Wolframs Tür wird geladen" );

print( "Definiere Listener" );

wolframstuer = $GOM.createGameObject("WolframsTuer")
wolframstuer.placeIntoScene();
wolframstuer.addAction(TurTalkAction.new())

print( "Wolframs Tür fertig geladen" );