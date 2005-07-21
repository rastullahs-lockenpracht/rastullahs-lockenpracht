# TODO Skript ausbauen in Hinblick auf Interaktion Held <-> Item.
# und Aktoren untereinander.
# Halt diese Klassen wie sie hier beschrieben sind auch benutzen.
# TODO Über Speichersparen nachdenken. Inwiefern können Instanzen
# einiger Klassen für Instanzen anderer Klassen nachgenutzt werden?
# Inwiefern ist Deferred Construction sinnvoll?
require 'globals.rb'
require 'actions.rb'
require 'items.rb'

class OpenDoorAction < RubyAction
  def initialize
    super("opendoor", "Tuer oeffnen");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo?(door, user)    
    not door.getBool("open");
  end
  
  def doAction(door, user, target)    
    doorActor = door.getActor(); 
    doorActor.getControlledObject().replaceAnimation("zu", "auf", 1.0, 1);
    knarzActor = doorActor.getChildByName(doorActor.getName()+"_knarzen");
    knarzActor.getControlledObject().play(0);
    # door.setOpen(true);
  end
end

class CloseDoorAction < RubyAction
  def initialize
    super("closedoor", "Tuer schliessen");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo?(door, user)    
    door.getBool("open");
  end
  
  def doAction(door, user, target)    
    doorActor = door.getActor();
    doorActor.getControlledObject.replaceAnimation("auf", "zu", 1.0, 1); 
    knarzActor = doorActor.getChildByName(doorActor.getName()+"_knarzen");
    knarzActor.getControlledObject().play(0);
    # door.setOpen(false);
  end
end

class Door < RubyItem
  def initialize(name, isOpen, canBeOpened)
    super(10, name, "Eine Tuer");

    doorActor = $AF.createMeshActor( name, "arc_tuer_01.mesh",  0, 0.0 ); #PhysicsManager::GT_BOX , 6.0);
    $CORE.log("door.rb - Aktor erstellt.");
    setActor(doorActor);
    $CORE.log("door.rb - Aktor gesetzt");
    soundActor = $AF.createSoundActor(name+"_knarzen","doorcreak.ogg");
    doorActor.attachToSlot(soundActor,"Bone01");
    $CORE.log("door.rb - Sound hinzugefuegt");
    
    setBool("open", isOpen);
    
    if (canBeOpened)
    	addAction(OpenDoorAction.new);
    	addAction(CloseDoorAction.new);
    	$CORE.log("door.rb - Aktionen hinzugefuegt.");
    else
    	addAction(OpenDoorAction.new, 7);
    	addAction(CloseDoorAction.new, 7);
    	$CORE.log("door.rb - Aktionen versteckt hinzugefuegt.");
    end
  end

end

