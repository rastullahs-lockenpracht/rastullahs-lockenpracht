load "embed.rb"
#require 'actorupdateanimationlistener.rb'
require 'jobs/soundjobs.rb'
require 'jobs/animationjobs.rb'
require 'gameobjectprops.rb'

class OpenDoorAction < Action
  def initialize
    super("opendoor", "Tür öffnen");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(door, user, target)    
    not door.isOpen
  end
  
  def doAction(door, user, target)    
    doorActor = door.getActor(); 
    
    #p "call fitToPose for zu"
    doorActor.getPhysicalThing().fitToPose("zu");

    PlayAnimation(doorActor, "auf", 1, true, true);
    PlaySound3d("doorcreak.ogg", doorActor.getPosition());
    door.setOpen(true);
  end
end

class CloseDoorAction < Action
  def initialize
    super("closedoor", "Tür schließen");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(door, user, target)
    door.isOpen
  end
  
  def doAction(door, user, target)    
    doorActor = door.getActor();
   
    #p "call fitToPose for auf"
    doorActor.getPhysicalThing().fitToPose("auf");

    PlayAnimation(doorActor, "zu", 1, true, true);
    PlaySound3d("doorcreak.ogg", doorActor.getPosition());
    door.setOpen(false);
  end
end

class Door < GameObject
  
  include GameObjectProperties
  
  def initialize(id)
    super(id);
	@_prop_openable = true;
	@_prop_open = false;
  end

  def placeIntoScene()
    super()
    addActions();
  end

  def addActions()
    @mOpenAction = OpenDoorAction.new()
    @mCloseAction = CloseDoorAction.new()
    if (@_prop_openable)
        addAction(@mOpenAction);
        addAction(@mCloseAction);
    else
        addAction(@mOpenAction, Action::ACT_DISABLED);
        addAction(@mCloseAction, Action::ACT_DISABLED);
    end

    doAction("opendoor") unless not @_prop_open
  end
  
  def setOpen(isOpen)
    @_prop_open = isOpen
    fireObjectStateChangeEvent();
  end
  
  def isOpen()
    @_prop_open
  end
  
  def getDefaultAction(actor)
     if (not @_prop_openable)
       super(actor)
     else
       if (@_prop_open)
         @mCloseAction
       else
         @mOpenAction
       end
     end
  end
end

