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
    super("opendoor", "Tür öffnen");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(door, user)    
    not door.isOpen
  end
  
  def doAction(door, user, target)    
    doorActor = door.getActor(); 
    doorActor.getControlledObject().replaceAnimation("zu", "auf", 1.0, 1);
    knarzActor = doorActor.getChildByName(doorActor.getName()+"_knarzen");
    knarzActor.getControlledObject().play();
    door.setOpen(true);
  end
end

class CloseDoorAction < RubyAction
  def initialize
    super("closedoor", "Tür schließen");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(door, user)    
    door.isOpen
  end
  
  def doAction(door, user, target)    
    doorActor = door.getActor();
    doorActor.getControlledObject.replaceAnimation("auf", "zu", 1.0, 1); 
    knarzActor = doorActor.getChildByName(doorActor.getName()+"_knarzen");
    knarzActor.getControlledObject().play();
    door.setOpen(false);
  end
end

class Door < RubyItem
  def initialize(name, description, isOpen, canBeOpened)
    super(10, name, description);

    doorActor = $AM.createMeshActor( name, "arc_tuer_01.mesh",  0, 0.0 ); #PhysicsManager::GT_BOX , 6.0);
    $CORE.log("door.rb - Aktor erstellt.");
    setActor(doorActor);
    $CORE.log("door.rb - Aktor gesetzt");
    soundActor = $AM.createSoundSampleActor(name+"_knarzen","doorcreak.ogg");
    doorActor.attachToSlot(soundActor,"Bone01");
    $CORE.log("door.rb - Sound hinzugefuegt");
    
    @mOpen = isOpen
    @mOpenAction = OpenDoorAction.new()
    @mCloseAction = CloseDoorAction.new()

    @mCanBeOpened = canBeOpened
    
    if (canBeOpened)
    	addAction(@mOpenAction);
    	addAction(@mCloseAction);
    	$CORE.log("door.rb - Aktionen hinzugefuegt.");
    else
    	addAction(@mOpenAction, Action::ACT_DISABLED);
    	addAction(@mCloseAction, Action::ACT_DISABLED);
    	$CORE.log("door.rb - Aktionen versteckt hinzugefuegt.");
    end

    @mDoor.doAction("opendoor") unless not @mOpen
  end
  
  def setOpen(isOpen)
    @mOpen = isOpen
    fireObjectStateChangeEvent();
  end
  
  def isOpen()
    @mOpen
  end
  
  def getDefaultAction(actor)
     if (not @mCanBeOpened)
       super(actor)
     else
       if (@mOpen)
         @mCloseAction
       else
         @mOpenAction
       end
     end
  end

end

