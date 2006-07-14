# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );

class WolframTalkAction < TalkAction
    def doAction(object, actor, target)
        if (RulesSubsystem.getSingleton().getQuestBook().getQuest("wolfram").getState() == Quest::SUCCEEDED) 
            super.doAction(object, actor, target)
	end
    end
end

print( "Wolfram Bot wird geladen" );

print( "Definiere Listener" );

wolfram = Creature.new("Wolfram", "Wolfram");
wolfram.setActor($AM.createMeshActor("Wolfram", "men_grim.mesh" ));
wolfram.getActor().placeIntoScene([-20.9051780700684, -6.61140871047974, 1.40333890914917],[ 0, 190.454895019531,0]);
wolfram.addAction(WolframTalkAction.new("Wolfram", "wolfram.xml"))
print( "Wolfram Bot fertig geladen" );