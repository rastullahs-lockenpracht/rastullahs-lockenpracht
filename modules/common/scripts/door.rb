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
  def canDo(door, user)    
    not door.isOpen
  end
  
  def doAction(door, user, target)    
    doorActor = door.getActor(); 
    
    #p "call fitToPose for zu"
    doorActor.getPhysicalThing().fitToPose("zu");

    PlayAnimation(doorActor, "auf");
    PlaySound3d("doorcreak.ogg", doorActor.getPosition());
    door.setOpen(true);
  end
end

class CloseDoorAction < Action
  def initialize
    super("closedoor", "Tür schließen");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(door, user)    
    door.isOpen
  end
  
  def doAction(door, user, target)    
    doorActor = door.getActor();
   
    #p "call fitToPose for auf"
    doorActor.getPhysicalThing().fitToPose("auf");

    PlayAnimation(doorActor, "zu");
    PlaySound3d("doorcreak.ogg", doorActor.getPosition());
    door.setOpen(false);
  end
end

class Door < GameObject
  
  include GameObjectProperties
  
  def initialize(id)
    super(id);
  end

  def setProperty(name, value)
    if (name == "sound")
      @_prop_Sound = value;
    elsif (name == "open")
      @_prop_Open = value;
    elsif (name == "openable")
      @_prop_CanBeOpened = value;
    else
      super(name, value)
    end
  end

  def placeIntoScene()
    super()
    addActions();
  end

  def addActions()
    @mOpenAction = OpenDoorAction.new()
    @mCloseAction = CloseDoorAction.new()
    if (@_prop_CanBeOpened)
        addAction(@mOpenAction);
        addAction(@mCloseAction);
    else
        addAction(@mOpenAction, Action::ACT_DISABLED);
        addAction(@mCloseAction, Action::ACT_DISABLED);
    end

    doAction("opendoor") unless not @_prop_Open
  end
  
  def setOpen(isOpen)
    @_prop_Open = isOpen
    fireObjectStateChangeEvent();
  end
  
  def isOpen()
    @_prop_Open
  end
  
  def getDefaultAction(actor)
     if (not @_prop_CanBeOpened)
       super(actor)
     else
       if (@_prop_Open)
         @mCloseAction
       else
         @mOpenAction
       end
     end
  end
end

