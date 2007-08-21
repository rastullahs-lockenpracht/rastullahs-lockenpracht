require 'steering.rb'

class TalkAction < Action
  def initialize()
    super("talk", "Reden")
  end

  def doAction(object, actor, target)
    bot = $AI.getBot(object.getName())
    if (bot.nil?)
      bot = $AI.loadBot(object.getName(), object.getDialogfile())
    end
    if ( not bot.nil? )
	  p "foo1"
      bot.setPlayerCharacter( actor );
	  p "foo2"
      bot.setNonPlayerCharacter( object );
	  p "foo3"
	  p "foo4"
	  agent = AgentManager::getSingleton().createAgent(object)
	  agent.pushState(RlScript::AST_DIALOG);
	  p "foo5"
	  agent.getCurrentState().setDialogPartner(
		AgentManager::getSingleton().createAgent(actor))
	  p "foo6"
    end
  end
end

module TalkTarget
  @mDialogfile;

  def initialize(id)
    super(id)
    addAction(TalkAction.new())
  end

  def setProperty(key, value)
    if (key == "dialogfile")
      @mDialogfile = value;
    else
      super(key, value)
    end
  end

  def getDialogfile()
    return @mDialogfile;
  end
end

module BehaviourBot
  def setProperty(key, value)
    if (key == "behaviours")
      print "behaviours:";
      p value;
    else
      super(key, value)
    end
    
    #TODO: Create behaviours like that:
    #
    #bot = $DS.loadBot("Sonja", "sonja.xml");
    #bot.setNonPlayerCharacter(sonja);
    #
    #agent = $AI.createAgent(bot);
    #agent.addSteeringBehaviour(DefaultIdleBehaviour.new);
    #agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
    #agent.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));
    
  end
end

class NPC < Person
  include TalkTarget
  include BehaviourBot
end
