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

wolfram = Creature.new("Wolfram", "Wolfram");
wolfram.setActor($AM.createMeshActor("Wolfram", "men_grim.mesh", PhysicsManager::GT_ELLIPSOID, 75.0));
wolfram.getActor().placeIntoScene([-20.99, -6.5, 1.55],[0.0, 180.0, 0.0]);

bot = $DS.loadBot("Wolfram", "wolfram.xml");
bot.setNonPlayerCharacter(wolfram);
wolfram.addAction(TalkAction.new("Wolfram", "wolfram.xml"))
#wolfram.addAction(WolframTalkAction.new("Wolfram", "wolfram.xml"))
agent = $AI.createAgent(bot);
agent.addSteeringBehaviour(DefaultIdleBehaviour.new);
#agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
agent.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));




tuer = $AM.getActor("WolframsTuer")
tuer.getControlledObject().startAnimation("auf",1.0,1)
tuer.getControlledObject().replaceAnimation("zu", "auf", 1.0, 1)
tuer.getPhysicalThing().fitToPose("zu")