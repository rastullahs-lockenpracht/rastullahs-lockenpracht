# Nur eingebettet starten
load( "embed.rb" );
require( "npc.rb" );
require( "door.rb" );

#class WolframTalkAction < TalkAction
#    def doAction(object, actor, target)
#        if (RulesSubsystem.getSingleton().getQuestBook().getQuest("wolfram").getState() == Quest::SUCCEEDED) 
#            super(object, actor, target)
#	    end
#    end

#    def canDo(enemy, user)    
#        RulesSubsystem.getSingleton().getQuestBook().getQuest("wolfram").getState() == Quest::SUCCEEDED
#    end
#end

print( "Wolfram Bot wird geladen" );

print( "Definiere Listener" );

$GOM.createGameObject("Wolfram").placeIntoScene();

tuer = $AM.getActor("WolframsTuer")
tuer.getControlledObject().startAnimation("auf",1.0,1)
tuer.getControlledObject().replaceAnimation("zu", "auf", 1.0, 1)
tuer.getPhysicalThing().fitToPose("zu")