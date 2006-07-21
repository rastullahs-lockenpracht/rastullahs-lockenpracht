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
wolframstuer.setActor($AM.createMeshActor("WolframsTuer", "arc_door_02.mesh" ));
wolframstuer.getActor().placeIntoScene([-20.8134, -6.61745, 1.91752],[0, 13, 0]);#<rotation qx="0.0" qy="0.785045" qz="0.0" qw="0.619439" />
wolframstuer.addAction(TurTalkAction.new("WolframsTuer", "wolframstuer.xml"))

print( "Wolframs Tür fertig geladen" );