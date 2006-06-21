# Nur eingebettet starten
load( "embed.rb" );
load( "npc.rb" );
load( "door.rb" );

class TurTalkAction < TalkAction
    def doAction(object, actor, target)
        if (RulesSubsystem.getSingleton().getQuestBook().getQuest("wolfram").getState() != Quest::SUCCEEDED) 
            super.doAction(object, actor, target)
	end
    end
end


print( "Wolframs Tür wird geladen" );

print( "Definiere Listener" );

wolframstuer = Creature.new("WolframsTuer", "WolframsTuer");
wolframstuer.setActor($AM.createMeshActor("WolframsTuer", "arc_tuer_01.mesh" ));
wolframstuer.getActor().placeIntoScene([-24.3, -6.6, 5.6]);
wolframstuer.addAction(TurTalkAction.new("WolframsTuer", "wolframstuer.xml"))

print( "Wolframs Tür fertig geladen" );