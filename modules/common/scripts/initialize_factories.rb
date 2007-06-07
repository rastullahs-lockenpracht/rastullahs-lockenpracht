class RubyClassFactory < UnifiedFactory
  def initialize()
    super();
  end

  def createRubyGameObject(classname, id)
    return Module.const_get(classname).new(id);
  end

  def createBehaviour(classname)
    return Module.const_get(classname).new();
  end

  def createTrigger(classname)
    return Module.const_get(classname).new();
  end

  def createEffect(classname, stufe)
    return Module.const_get(classname).new(id);
  end
end

factory = RubyClassFactory.new()
$GOM.setGameObjectFactory(factory);
$AI.setBehaviourFactory(factory);
$SCRIPT.setTriggerFactory(factory)
EffectFactoryManager.getSingleton().setEffectFactory(factory)
