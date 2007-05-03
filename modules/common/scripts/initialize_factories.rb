class RubyGameObjectFactory < GameObjectFactory
  def initialize()
    super();
  end

  def createRubyGameObject(classname, id)
    return Module.const_get(classname).new(id);
  end
end

class RubyBehaviourFactory < BehaviourFactory
  def initialize()
    super();
  end

  def createBehaviour(classname)
    return Module.const_get(classname).new();
  end
end

class RubyTriggerFactory < TriggerFactory
  def initialize()
    super();
  end

  def createTrigger(classname)
    return Module.const_get(classname).new();
  end
end

$GOM.setGameObjectFactory(RubyGameObjectFactory.new());
$AI.setBehaviourFactory(RubyBehaviourFactory.new());
$SCRIPT.setTriggerFactory(RubyTriggerFactory.new())
