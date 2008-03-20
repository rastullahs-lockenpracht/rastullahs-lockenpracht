require "embed.rb"
require "areahelper.rb"

def changeMap(mapLoader, mapNames)
	characterId = $UI.getActiveCharacter().getId()
	CreatureControllerManager.getSingleton().detachController($UI.getActiveCharacter())
	$UI.setActiveCharacter(nil)
	#$SCRIPT.logError(mapNames.type().to_s)
	mapLoader.requestSceneChange(mapNames)
end

class MapChangeTrigger < Trigger
  def initialize(classname, name)
    super(classname, name, false);
  end

  def activate()
	changeMap($MAPLOADER, @maps)
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
  def setMaps(maps)
	@maps = maps
  end
  def getAllProperties()
    ps = super();
    return ps
  end
end

def CreateMapChangeTrigger(name, pos, orientation, size, maps)
	unless ( ZoneManager.getSingleton().getZone("MapChangeZone_" + name) == nil )
		raise ArgumentError, "A MapChangeTrigger with name " + name + " already exists!", caller
	end
	trigger = $SCRIPT.getTriggerFactory().createTrigger("MapChangeTrigger", "MapChangeTrigger" + name)
	trigger.setMaps(maps)
	zone = ZoneManager.getSingleton().createZone("MapChangeZone_" + name, true)
	ZoneManager.getSingleton().addAreaToZone(
    "MapChangeZone_" + name, 
    size, PhysicsManager::GT_BOX, pos, [0,0,0], orientation, 0.2, RlScript::QUERYFLAG_PLAYER);
	trigger.setProperty("zone","MapChangeZone_" + name)
	zone.addTrigger(trigger)
	zone.getTriggers().methods()
end