require 'steering.rb'

class TalkAction < Action
  def initialize()
    super("talk", "Reden")
  end

  def doAction(object, actor, target)
    npcs = Array.new();
    npcs.push(object);
    npcs += object.getCreaturesNearby(1, false);
    dialog = DialogManager::getSingleton().createDialog(object.getDialog(), npcs);  
	  agent = AgentManager::getSingleton().createAgent(object);
	  agent.pushState(RlScript::AST_DIALOG);
    agent.getCurrentState().setDialog(dialog);
  end
end

module TalkTarget
  @mDialog;

  def initialize(id)
    super(id)
    addAction(TalkAction.new())
  end

  def setProperty(key, value)
    if (key == "dialog")
      @mDialog = value;
    else
      super(key, value)
    end
  end

  def getDialog()
    return @mDialog;
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
    #agent = $AGM.createAgent(bot);
    #agent.addSteeringBehaviour(DefaultIdleBehaviour.new);
    #agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);
    #agent.addSteeringBehaviour(DialogBehaviour.new($heroVehicle));
    
  end
end

class NPC < Creature
  include TalkTarget
  include BehaviourBot
  
  def getCreaturesNearby(range, includePlayers)
    gos = $GOM.getAllGameObjects();
    gos.each { |go|
      if ((go.getPosition() - self.getPosition()).getLength() <= range) and
          (includePlayers || !PartyManager.getSingleton().isInParty(go))
        gos -= go;
      end
    }
    return gos;
  end
end