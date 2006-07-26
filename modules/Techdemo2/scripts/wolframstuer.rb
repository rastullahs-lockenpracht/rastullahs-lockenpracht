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
        RulesSubsystem.getSingleton().getQuestBook().getQuest("wolfram").getState() != Quest::SUCCEEDED
    end
end


print( "Wolframs Tür wird geladen" );

print( "Definiere Listener" );

wolframstuer = Creature.new("WolframsTuer", "WolframsTuer");
#wolframstuer.setActor($AM.createMeshActor("WolframsTuer", "arc_door_02.mesh" ));
wolframstuer.setActor($AM.createMeshActor("WolframsTuer", "arc_door_02.mesh", PhysicsManager::GT_BOX, 0.0));
wolframstuer.getActor().placeIntoScene([-20.8134, -6.61745, 1.91752],[0, 13, 0]);
wolframstuer.addAction(TurTalkAction.new("WolframsTuer", "wolframstuer.xml"))

print( "Wolframs Tür fertig geladen" );