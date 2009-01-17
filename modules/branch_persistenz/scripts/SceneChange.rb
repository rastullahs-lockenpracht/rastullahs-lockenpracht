require "embed.rb"
require "areahelper.rb"

def changeScene(scene)
#	characterId = PartyManager.getSingleton().getActiveCharacter().getId()
#	CreatureControllerManager.getSingleton().detachController(PartyManager.getSingleton().getActiveCharacter())
#	PartyManager.getSingleton().setActiveCharacter(nil)
	#$SCRIPT.logError(mapNames.type().to_s)
#	mapLoader.requestSceneChange(mapNames)
	SceneManager::getSingleton().loadScene(scene, true);
end

class SceneChangeTrigger < Trigger
  def initialize(classname, name)
    super(classname, name, false);
  end

  def activate()
	changeScene(@scene)
    return false
  end
  def deactivate()
    return false
  end
  def setProperty(name, value)
    super(name, value)
  end
  def getProperty(name)
	super(name)
  end
  def setScene(scene)
	@scene = scene
  end
  def getAllProperties()
    ps = super();
    return ps
  end
end

def CreateSceneChangeTrigger(name, pos, orientation, size, scene)
	unless ( ZoneManager.getSingleton().getZone("SceneChangeZone_" + name) == nil )
		raise ArgumentError, "A SceneChangeTrigger with name " + name + " already exists!", caller
	end
	trigger = $SCRIPT.getTriggerFactory().createTrigger("SceneChangeTrigger", "SceneChangeTrigger" + name)
	trigger.setScene(scene)
	zone = ZoneManager.getSingleton().createZone("SceneChangeZone_" + name, true)
	ZoneManager.getSingleton().addAreaToZone(
	    "SceneChangeZone_" + name, 
	    size, PhysicsManager::GT_BOX, pos, [0,0,0], orientation, 0.2, RlScript::QUERYFLAG_PLAYER);
	trigger.setProperty("zone","SceneChangeZone_" + name)
	zone.addTrigger(trigger)
end