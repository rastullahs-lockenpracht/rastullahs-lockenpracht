load "effects/ruestung.rb"
load "effects/lifestateeffects.rb"
load "triggers/test.rb"
load "triggers/limitedtimestrigger.rb"

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

  def createTrigger(classname, name)
    return Module.const_get(classname).new(classname, name);
  end

  def createEffect(classname, stufe)
    return Module.const_get(classname).new(stufe);
  end
end

factory = RubyClassFactory.new()
$GOM.setGameObjectFactory(factory);
$AGM.setBehaviourFactory(factory);
$SCRIPT.setTriggerFactory(factory)
EffectFactoryManager.getSingleton().setEffectFactory(factory)
