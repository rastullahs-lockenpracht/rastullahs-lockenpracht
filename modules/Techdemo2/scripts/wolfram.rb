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
wolfram.setActor($AM.createMeshActor("Wolfram", "men_grim.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
wolfram.getActor().placeIntoScene([-22.0, -6.5, -3.0],[ 0, 190.454895019531,0]);

bot = $DS.loadBot("Wolfram", "wolfram.xml");
bot.setNonPlayerCharacter(wolfram);
wolfram.addAction(WolframTalkAction.new("Wolfram", "wolfram.xml"))
agent = $AI.createAgent(bot);
agent.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));

print( "Wolfram Bot fertig geladen" );