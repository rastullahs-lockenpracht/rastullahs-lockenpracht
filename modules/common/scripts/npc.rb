require 'steering.rb'

class TalkAction < Action
  def initialize()
    super("talk", "Reden")
  end

  def doAction(object, actor, target)
    dialog = DialogManager::getSingleton().createDialog(object.getDialog(), object, actor)  
	  agent = AgentManager::getSingleton().createAgent(object)
	  agent.pushState(RlScript::AST_DIALOG);
	  agent.getCurrentState().setDialogPartner(
  		AgentManager::getSingleton().createAgent(actor))
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

class NPC < Person
  include TalkTarget
  include BehaviourBot
end
