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
    super("Öffnen", "Tür öffnen");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo?(door, user)    
    not door.open?;
  end
  
  def doAction(door, user, target)    
    doorActor = door.getActor();    
    doorActor.yaw(90.0)
    # door.setOpen(true);
  end
end

class CloseDoorAction < RubyAction
  def initialize
    super("Schließen", "Tür schließen");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo?(door, user)    
    door.open?;
  end
  
  def doAction(door, user, target)    
    doorActor = door.getActor();
    doorActor.yaw(-90.0)
    # door.setOpen(false);
  end
end

class Door < RubyItem
  def initialize(name, isOpen, canBeOpened)
    super(10, name, "Eine Tuer");

    doorActor = $AF.createMeshActor(name, "door01.mesh", 0, -1.0);
    $CORE.log("door-actor erstellt.");
    setActor(doorActor);
    $CORE.log("actor gesetzt");
    
    @open = isOpen;    
    
    if (canBeOpened)
    	addAction(OpenDoorAction.new);
    	addAction(CloseDoorAction.new);
    	$CORE.log("Aktionen hinzugefuegt.");
    end
  end
  
  def setOpen(open)
    @open = open;
  end
  
  def open?
    @open;
  end
end

