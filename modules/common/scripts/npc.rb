load( "embed.rb" );

class TalkAction < Action
  def initialize()
    super("talk", "Reden")
  end

  def doAction(object, actor, target)
    bot = $DS.getBot(object.getName())
    if (bot.nil?)
      bot = $DS.loadBot(object.getName(), object.getDialogfile())
    end
    if ( not bot.nil? )
      bot.setPlayerCharacter( actor );
      bot.setNonPlayerCharacter( object );
      WindowFactory.getSingleton().showDialog(bot);
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